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
import http.server
import socketserver

s_isGDAlive = False
s_lastHeartbeatTime = time.time()

class RequestHandler(http.server.SimpleHTTPRequestHandler):
    def do_GET(self):
        global s_isGDAlive
        global s_lastHeartbeatTime
        try:
            if self.client_address[0] != "127.0.0.1":
                logging.warn("SECURITY WARNING: The ESH daemon got a request from an address other than your PC! Make sure it isn't exposed to the internet!")
                print("Odd address: %s" % self.client_address)
                pass
            else:
                if self.path == "/heartbeat" and self.headers.get("User-Agent") == "eye-strain-helper":
                    logging.debug("[ESH-Integration] Got heartbeat!")
                    s_isGDAlive = True
                    s_lastHeartbeatTime = time.time()
                    self.send_response_only(200, "Got heartbeat!")

        except:
            pass
    def send_error(self):
        pass

    def log_request():
        pass

    def log_error():
        pass


def try_startDaemon():
    with socketserver.TCPServer(("", 7289), RequestHandler) as httpd:
        logging.debug("Started HTTP daemon!")
        httpd.serve_forever()

def initListenServer():
    logging.debug("[ESH-Integration] Starting HTTP Daemon...")
    global s_isGDAlive
    try:
        try_startDaemon()
    except:
        logging.error("[ESH-Integration] Failed to start HTTP daemon. Retrying in 15 seconds.")
        time.sleep(15)
        ct = 0
        while ct < 5:
            try:
                try_startDaemon()
                break
            except:
                ct += 1
                if ct <= 5:
                    logging.error("[ESH-Integration] Failed to start HTTP daemon. Retrying in 15 seconds. (Attempt %s/5)" % (ct+1))
                    time.sleep(15)
        

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