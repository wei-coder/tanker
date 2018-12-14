###############################################################################################################                                                          
# Program Name: Browser_Client_Coder.html                                     
# ================================     
# This code is for controlling a robot by a web browser using web sockets                            
# http://www.7gp.cn/                                                                
# History
# ------------------------------------------------
# Author     Comments
# Ææ¹ûÅÉ¹¤·»     
#                                                                  
# These files have been made available online through a Creative Commons Attribution-ShareAlike 3.0  license.
# (http://creativecommons.org/licenses/by-sa/3.0/)           
#
###############################################################################################################

#
# PREREQUISITES
#	Tornado Web Server for Python
#
# TROUBLESHOOTING:
#	Don't use Ctrl+Z to stop the program, use Ctrl+c.
#	If you use Ctrl+Z, it will not close the socket and you won't be able to run the program the next time.
#	If you get the following error:
#		"socket.error: [Errno 98] Address already in use "
#	Run this on the terminal:
#		"sudo netstat -ap |grep :9093"
#	Note down the PID of the process running it
#	And kill that process using:
#		"kill pid"
#	If it does not work use:
#		"kill -9 pid"
#	If the error does not go away, try changin the port number '9093' both in the client and server code
import time,sys,json

# Import the ArmRobot.py file (must be in the same directory as this file!).
import ArmRobot

import threading
import tornado.ioloop
import tornado.web
import tornado.websocket
import tornado.template
import tornado.escape

LEFT_TRIM   = 0
RIGHT_TRIM  = 0


robot = ArmRobot.ArmRobot(addr=0x6f,left_id=3,right_id=4,left_trim=LEFT_TRIM, right_trim=RIGHT_TRIM)

c=0
#Initialize TOrnado to use 'GET' and load index.html
class MainHandler(tornado.web.RequestHandler):
  def get(self):
    loader = tornado.template.Loader(".")
    self.write(loader.load("index.html").generate())

#Code for handling the data sent from the webpage
class WSHandler(tornado.websocket.WebSocketHandler):
	def open(self):
		print 'connection opened...'
	def on_message(self, message):      # receives the data from the webpage and is stored in the variable message
		global c
		print 'received:', message        # prints the revived from the webpage 
		decodejson = json.loads(message)
		c=decodejson['eventType']
		v=decodejson['eventValue']
		print 'eventType:',c
		if c == 8 :
		  print "Running Forward"
		  robot.forward(150, 1.0)
		elif c == 2 :
		  print "Running Reverse"
		  robot.backward(150, 3.0)
		elif c == 4 :
		  print "Turning Right"
		  robot.right(200, 1.5)
		elif c == 6 :
		  print "Turning Left"
		  robot.left(200, 1.0)
		elif c == 5 :
		  print "Stopped"
		  robot.stop()      # Stop the robot from moving.
		  
		elif c == 1 :
		  print "Arm Claw:",v
		  robot.armClaw(v)
		elif c == 3:
		  print "Arm Waist:",v
		  robot.armWaist(v)
		elif c == 7:
		  print "Arm Left:",v
		  robot.armLeft(v)
		elif c == 9 :
		  print "Arm Rigth:",v
		  robot.armRight(v)		  		  		  		
		print "Values Updated"
	def on_close(self):
		robot.stop()      # Stop the robot from moving.
		print 'connection closed...'

application = tornado.web.Application([
  (r'/ws', WSHandler),
  (r'/', MainHandler),
  (r"/(.*)", tornado.web.StaticFileHandler, {"path": "./resources"}),
])

class myThread (threading.Thread):
    def __init__(self, threadID, name, counter):
        threading.Thread.__init__(self)
        self.threadID = threadID
        self.name = name
        self.counter = counter
    def run(self):
        print "Ready"
        while running:
            time.sleep(.2)              # sleep for 200 ms

if __name__ == "__main__":



	running = True
	thread1 = myThread(1, "Thread-1", 1)
	thread1.setDaemon(True)
	thread1.start()  
	application.listen(9093)          	#starts the websockets connection
	tornado.ioloop.IOLoop.instance().start()
  

