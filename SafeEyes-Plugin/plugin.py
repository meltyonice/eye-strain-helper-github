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
from threading import Thread
import time
import http.server
import socketserver

s_isGDAlive = False
s_lastHeartbeatTime = time.time()

class ESHIHandler(http.server.SimpleHTTPRequestHandler):
    
    def do_GET(self):
        global s_lastHeartbeatTime
        global serverThread2
        global shm
        global context
        global session
        global s_isGDAlive
        if self.path == "/heartbeat":
            s_isGDAlive = True
            s_lastHeartbeatTime = time.time()
            self.send_response(200)
            self.send_header("Content-type", "application/none")
            self.end_headers()
        return

def initListenServer():
    logging.debug("[ESH-Integration] Starting HTTP Daemon...")
    global s_lastHeartbeatTime
    global serverThread2
    global shm
    global context
    global session
    global s_isGDAlive
    
    Handler = ESHIHandler
    with socketserver.TCPServer(("127.0.0.1", 7289), Handler) as httpd:
        logging.debug("[ESH-Integration] HTTP Daemon is up!")
        httpd.serve_forever()
    
        

def init(ctx, safeeyes_config, plugin_config):
    """
    Initialize the plugin.
    """
    logging.debug("[ESH-Integration] Initializing...")
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
    global s_lastHeartbeatTime
    
    # notification_expire_time = short_break_interval if break_obj.is_short_break() else long_break_interval
    logging.debug(s_isGDAlive)
    if s_isGDAlive:
        logging.debug(time.time()-s_lastHeartbeatTime)
    if time.time()-s_lastHeartbeatTime > 30 and s_isGDAlive:
        s_isGDAlive = False
    
    if s_isGDAlive:
        return True