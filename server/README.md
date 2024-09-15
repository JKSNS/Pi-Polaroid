# Client Lab Server

## Setup

```bash
python3 -m venv ./.venv         # Create virtual environment
source .venv/bin/activate       # Activate virtual environment
pip install -r requirements.txt # Install dependencies
mkdir photos                    # Make the folder for photos
python app.py                   # Run the server
```

Python 3.12 has a compiling issue that fails to build the wheel for multi_dict and several others so the workaround for this is to use: 

```bash
pip cache purge
apt install python3.12-dev
sudo apt-get install python3-dev build-essential
sudo apt-get install libssl-dev libffi-dev
pip install --upgrade pip setuptools wheel
pip install aiohttp==3.9.0b0
```

This create two servers:

  - The image server that the clients connect to to upload their images. This runs on port 2240.
    
  - The web server that you can view your images on. This runs on port 2241.

The web server stores images based on homework IDs. For example, if your homework ID was ABC123456, then they would navigate to `http://YOUR-HOST-IP:2241/ABC123456`.

## Running in the Background

You can install a systemd service to have the server run on start up and restart automatically if it crashes. The file is `pipolaroid.service` and can be installed running the following commands:

```bash
sudo cp pipolaroid.service /etc/systemd/system/
sudo systemctl daemon-reload
sudo systemctl start pipolaroid.service   # Start service
sudo systemctl enable pipolaroid.service  # Have service run on boot

```
