# Client Lab Server

## Setup

```bash
python3 -m venv ./.venv         # Create virtual environment
source .venv/bin/activate       # Activate virtual environment
pip install -r requirements.txt # Install dependencies
mkdir photos                    # Make the folder for photos
python app.py                   # Run the server
```

Python 3.12 has a compoiling issue that fails to build the wheel for multi_dict and several others so the workaround for this is to use Python 3.10.11. 

To install Python 3.10 on Ubuntu, you have several methods to choose from. Below are detailed steps for each method:

---

### **Method 1: Using the Deadsnakes PPA**

The Deadsnakes PPA is a popular repository that provides newer and older versions of Python not available in the default Ubuntu repositories.

1. **Update Package List:**

   ```bash
   sudo apt update
   ```

2. **Install Prerequisites:**

   ```bash
   sudo apt install software-properties-common
   ```

3. **Add the Deadsnakes PPA:**

   ```bash
   sudo add-apt-repository ppa:deadsnakes/ppa
   sudo apt update
   ```

4. **Install Python 3.10:**

   ```bash
   sudo apt install python3.10
   ```

5. **Verify the Installation:**

   ```bash
   python3.10 --version
   ```

**Note:** The Deadsnakes PPA typically provides the latest patch versions of Python 3.10. However, it might not always have the specific sub-version 3.10.11. To check the exact version available:

```bash
apt-cache policy python3.10
```

If the available version is not 3.10.11, consider using Method 2 or 3.

---

### **Method 2: Building Python 3.10.11 from Source**

Compiling Python from source allows you to install a specific version.

1. **Install Build Dependencies:**

   ```bash
   sudo apt update
   sudo apt install -y build-essential \
       zlib1g-dev \
       libncurses5-dev \
       libgdbm-dev \
       libnss3-dev \
       libssl-dev \
       libreadline-dev \
       libffi-dev \
       libsqlite3-dev \
       wget \
       libbz2-dev \
       liblzma-dev \
       tk-dev \
       uuid-dev
   ```

2. **Download Python 3.10.11 Source Code:**

   ```bash
   wget https://www.python.org/ftp/python/3.10.11/Python-3.10.11.tgz
   ```

3. **Extract the Archive:**

   ```bash
   tar -xf Python-3.10.11.tgz
   ```

4. **Compile and Install Python:**

   ```bash
   cd Python-3.10.11
   ./configure --enable-optimizations
   make -j $(nproc)
   sudo make altinstall
   ```

   - The `--enable-optimizations` flag optimizes the Python binary by running multiple tests, which can take some time.
   - `make altinstall` is used to prevent overwriting the default `python3` binary in `/usr/bin/python3`.

5. **Verify the Installation:**

   ```bash
   python3.10 --version
   ```

**Note:** This method installs `python3.10` without affecting the system's default Python interpreter.

---

### **Method 3: Using Pyenv**

Pyenv is a Python version management tool that allows you to easily switch between multiple versions.

1. **Install Dependencies:**

   ```bash
   sudo apt update
   sudo apt install -y build-essential curl libssl-dev zlib1g-dev \
       libbz2-dev libreadline-dev libsqlite3-dev wget llvm \
       libncurses5-dev xz-utils tk-dev libxml2-dev libxmlsec1-dev libffi-dev liblzma-dev
   ```

2. **Install Pyenv:**

   ```bash
   curl https://pyenv.run | bash
   ```

3. **Update Shell Configuration:**

   Add the following to your `~/.bashrc` (or `~/.bash_profile` if you're using that):

   ```bash
   # Pyenv Configuration
   export PYENV_ROOT="$HOME/.pyenv"
   export PATH="$PYENV_ROOT/bin:$PATH"
   eval "$(pyenv init --path)"
   eval "$(pyenv init -)"
   ```

   Reload the shell:

   ```bash
   exec $SHELL
   ```

4. **Install Python 3.10.11 with Pyenv:**

   ```bash
   pyenv install 3.10.11
   pyenv global 3.10.11
   ```

5. **Verify the Installation:**

   ```bash
   python --version
   ```

**Note:** Pyenv installs Python versions in your home directory and doesn't interfere with the system Python.

---

### **Additional Information**

- **Virtual Environments:** Regardless of the method you choose, it's good practice to use virtual environments for your Python projects. This keeps project dependencies isolated.

  ```bash
  python3.10 -m venv myenv
  source myenv/bin/activate
  ```

- **Managing Multiple Versions:** If you need to switch between different Python versions, Pyenv (Method 3) is the most flexible approach.

- **System Stability:** Avoid removing the default system Python, as it may cause system tools to fail.

---

### **Troubleshooting**

- **Missing Libraries:** If you encounter errors during compilation, make sure all dependencies are installed.

- **Permission Issues:** If you get permission errors, ensure you're using `sudo` where necessary.

---


This create two servers:

  - The image server that the clients connect to to upload their images. This runs on port 2240.
    
  - The web server that students can view their images. This runs on port 2241.

The web server stores students' images based on their homework IDs (as reported in LS). For example, if a student's homework ID was ABC123456, then they would navigate to `http://ecen224.byu.edu:2241/ABC123456` (assuming the host is ecen224.byu.edu).

Between semesters, you might need to delete the photos folder, except for `photos/123456789`.

## Running in the Background

You can install a systemd service to have the server run on start up and restart automatically if it crashes. The file is `client_lab_server.service` and can be installed running the following commands:

```bash
sudo cp client_lab_server.service /etc/systemd/system/
sudo systemctl daemon-reload
sudo systemctl start client_lab_server.service   # Start service
sudo systemctl enable client_lab_server.service  # Have service run on boot

```
