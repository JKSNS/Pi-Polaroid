import argparse
import asyncio
import functools
import logging
import logging.handlers
from pathlib import Path
import re
import os
from aiohttp import web
import aiohttp_jinja2
import arrow
import jinja2

# Set up logger
LOGGER = logging.getLogger(__name__)
LOGGER.setLevel(level=logging.INFO)

# Set up output to file and terminal
LOGGER.addHandler(logging.handlers.TimedRotatingFileHandler("client_lab_server.log", when="midnight"))
LOGGER.addHandler(logging.StreamHandler())

# Constants
HOMEWORK_ID_LENGTH = 9
IMAGE_SIZE = 49206
ROOT_DIR = "photos"

# Utility functions for error handling and logging
async def send_error(writer, message):
    writer.write(message)
    await writer.drain()
    writer.close()
    await writer.wait_closed()
    LOGGER.info(f"Error sent: {message.decode()}")

# TCP connection handler
async def handle_client(reader, writer, delay=0, timeout=5):
    LOGGER.info("Client connected...")

    # Read in homework ID
    try:
        homework_id = await asyncio.wait_for(reader.readexactly(HOMEWORK_ID_LENGTH), timeout=timeout)
        homework_id = homework_id.decode()
    except (TimeoutError, asyncio.IncompleteReadError, UnicodeDecodeError) as e:
        LOGGER.error(f"Error receiving homework ID: {str(e)}")
        await send_error(writer, b"ERROR: Invalid or missing homework ID")
        return

    # Validate homework ID
    if not re.fullmatch("[A-F0-9]{9}", homework_id):
        LOGGER.warning(f"Invalid homework_id: {homework_id}")
        await send_error(writer, f"ERROR: Invalid homework ID {homework_id}".encode())
        return

    LOGGER.info(f"Received valid homework ID {homework_id}. Now receiving image data...")

    if delay:
        LOGGER.info(f"Sleeping for {delay} seconds...")
        await asyncio.sleep(delay)

    # Read image data
    try:
        image_data = await asyncio.wait_for(reader.readexactly(IMAGE_SIZE), timeout=timeout)
    except (TimeoutError, asyncio.IncompleteReadError) as e:
        LOGGER.error(f"Error receiving image data: {str(e)}")
        await send_error(writer, b"ERROR: Unable to read image data")
        return

    LOGGER.info(f"Received data from {homework_id}: {image_data[:50]}")

    # Validate BMP file header
    if image_data[:2] != b"BM":
        LOGGER.warning("Invalid BMP file: It doesn't start with BM.")
        await send_error(writer, b"ERROR: BMP file does not start with BM")
        return

    # Save the image
    path = Path(ROOT_DIR) / homework_id
    path.mkdir(parents=True, exist_ok=True)
    file_name = path / f"{arrow.now()}.bmp"

    try:
        with open(file_name, "wb") as f:
            f.write(image_data)
        LOGGER.info(f"Successfully saved image to {file_name}")
    except Exception as e:
        LOGGER.error(f"Error saving image: {str(e)}")
        await send_error(writer, b"ERROR: Failed to save image")
        return

    writer.write(b"SUCCESS")
    await writer.drain()
    writer.close()
    await writer.wait_closed()

# TCP server setup
async def image_server(port, delay, timeout):
    server = await asyncio.start_server(
        functools.partial(handle_client, delay=delay, timeout=timeout), "YOUR-MACHINE-IP", port
    )
    LOGGER.info(f"Running on tcp://YOUR-MACHINE-IP:{port} with {delay} s of delay")

    async with server:
        await server.serve_forever()

# HTTP handler to display images
@aiohttp_jinja2.template("index.html")
async def index(request):
    homework_id = request.match_info["homework_id"]
    path = Path(ROOT_DIR) / homework_id
    files = sorted(path.glob("*.bmp"), reverse=True)
    return {"homework_id": homework_id, "file_names": files}

# HTTP handler to delete images
async def delete_image(request):
    homework_id = request.match_info["homework_id"]
    file_name = request.match_info["file_name"]
    image_path = Path(ROOT_DIR) / homework_id / file_name

    try:
        if image_path.exists():
            os.remove(str(image_path))
            LOGGER.info(f"Deleted image: {image_path}")
        else:
            LOGGER.warning(f"File not found for deletion: {image_path}")
    except Exception as e:
        LOGGER.error(f"Error deleting file {image_path}: {e}")
        return web.Response(text=f"Error deleting file: {e}", status=500)

    LOGGER.info(f"Redirecting to /photos/{homework_id} after deletion.")
    raise web.HTTPFound(f'/photos/{homework_id}')

# Background tasks setup
async def start_background_tasks(app):
    app["image_server"] = asyncio.create_task(
        image_server(app["image_port"], app["delay"], app["timeout"])
    )

async def cleanup_background_tasks(app):
    app["image_server"].cancel()
    await app["image_server"]

# Utility functions for formatting time
def get_time(input):
    try:
        return arrow.get(input.stem).to("US/Mountain").format("MMM D, YYYY h:mm A")
    except arrow.parser.ParserError:
        return ""

def get_relative_time(input):
    try:
        return arrow.get(input.stem).humanize()
    except arrow.parser.ParserError:
        return ""

# Main function to run the web and TCP server
def run(image_port=2240, web_port=2241, delay=0, timeout=5):
    app = web.Application()
    app["image_port"] = image_port
    app["delay"] = delay
    app["timeout"] = timeout

    # Setup templates
    aiohttp_jinja2.setup(
        app,
        loader=jinja2.FileSystemLoader("templates"),
        filters=[("get_time", get_time), ("get_relative_time", get_relative_time)],
    )

    # Setup application routes
    app.add_routes([web.get("/photos/{homework_id:[A-F0-9]{9}}", index)])
    app.add_routes([web.post('/delete/{homework_id}/{file_name}', delete_image)])
    app.router.add_static("/photos", path="photos/")
    app.on_startup.append(start_background_tasks)
    app.on_cleanup.append(cleanup_background_tasks)
    web.run_app(app, port=web_port)

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--image-server-port", type=int, default=2240)
    parser.add_argument("--web-server-port", type=int, default=2241)
    parser.add_argument("--delay", type=float, default=0, help="Delay in seconds.")
    parser.add_argument("--timeout", type=float, default=None, help="Timeout in seconds.")
    args = parser.parse_args()

    run(args.image_server_port, args.web_server_port, args.delay, args.timeout)
