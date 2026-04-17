# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

import logging
import os
from safeeyes.model import State
from threading import *
import subprocess
import time
import ipckit
import subprocess

s_isGDAlive = False
s_lastHeartbeatTime = time.time()

def launchRSListener():
    os.chdir(".config/safeeyes/plugins/esh-integration/")
    subprocess.run("./eshlistener")

def initListenServer():
    logging.debug("[ESH-Integration] Starting HTTP Daemon...")
    global s_isGDAlive
    global s_lastHeartbeatTime
    global serverThread2
    global shm
    #logging.debug(os.getcwd())
    try:
        shm = ipckit.SharedMemory.create("eshs2rs", 512)
    except FileExistsError:
        shm = ipckit.SharedMemory.open("eshs2rs")
        shm.write(0,bytes(512))
    serverThread2 = Thread(target=launchRSListener)
    serverThread2.daemon = True
    serverThread2.start()
    time.sleep(0.001)
    sC = True
    if shm.read(2,2) != b"11":
        logging.debug("Couldn't detect listen server!")
        sC = False
    while sC:
        time.sleep(0.001)
        if shm.read(6,9) == b"HEARTBEAT":
            logging.debug("Got heartbeat!")
            shm.write(6,bytes(9))
        

def init(ctx, safeeyes_config, plugin_config):
    """
    Initialize the plugin.
    """
    global context
    global session
    global s_isGDAlive
    context = ctx
    serverThread = Thread(target=initListenServer)
    serverThread.daemon = True
    serverThread.start()

def disable() -> None:
    subprocess.run(["killall", "eshlistener"])

def on_stop():
    subprocess.run(["killall", "eshlistener"])

def on_start_break(break_obj):

    logging.debug(break_obj)

    global s_isGDAlive

    #FOR TESTING PURPOSES: DELETE ME LATER!
    #client = ipckit.NamedPipe.connect("EyeStrainHelperPersistentIPCPipe")
    #client.write(b"HBEATCLI")
    
    # notification_expire_time = short_break_interval if break_obj.is_short_break() else long_break_interval
    #if s_isGDAlive:
    logging.debug(time.time()-s_lastHeartbeatTime)
    if time.time()-s_lastHeartbeatTime > 30 and s_isGDAlive:
        s_isGDAlive = False
    
    if s_isGDAlive:
        return True