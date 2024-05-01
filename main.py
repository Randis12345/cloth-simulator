from math import sqrt,pi
import pygame as p

def mvec(vec,factor):
	return [vec[0]*factor,vec[1]*factor]
def getmag(vec):
	return sqrt(vec[0]**2+vec[1]**2)
def setmag(vec,factor):
	multi=factor/getmag(vec)
	return mvec(vec,multi)
def drawpoint(pos):
	return addvec(mvec(pos,zoom),offset)
def addvec(vec1,vec2):
	return [vec1[0]+vec2[0],vec1[1]+vec2[1]]

class Anchor():
	def __init__(s,pos):
		s.updatePos(pos)
	def updatePos(s,pos):
		s.pos = pos
	def applyForce(s,force): return

class Point():
	def __init__(s,pos):
		s.pos = pos
		s.vel = [0,0]
		s.mass = 0.5
		s.connections = []
	def vectorto(s,other):
		return [other.pos[0]-s.pos[0],other.pos[1]-s.pos[1]]
	def distanceto(s,other):
		return getmag(s.vectorto(other))
	def addconnection(s,other,lenoverwrite=0):
		if lenoverwrite:
			length = lenoverwrite
		else:
			length = s.distanceto(other)
		s.connections.append({
			"ref":other,
			"length": length,
			"k": 100
			})
	def applyForce(s,force):
		s.vel[0]+=force[0]/s.mass*dt
		s.vel[1]+=force[1]/s.mass*dt
	def update(s):
		for connection in s.connections:
			force = s.vectorto(connection["ref"])
			dist = getmag(force)
			if dist <= connection["length"]: continue
			dist-=connection["length"]
			forcemag = dist*connection["k"]
			force = setmag(force,forcemag)
			s.applyForce(force)
			connection["ref"].applyForce(mvec(force,-1))
		s.applyForce([0,gravityaccl*s.mass])
		s.vel = mvec(s.vel,0.99)
	def updatePos(s):
		s.pos[0]+=s.vel[0]*dt
		s.pos[1]+=s.vel[1]*dt
	def draw(s):
		drawpos = drawpoint(s.pos)
		#p.draw.circle(screen,"black",drawpos,5*zoom)
		for connection in s.connections:
			p.draw.line(screen,"black",drawpos,drawpoint(connection["ref"].pos))


def createsheet(w,h,dist,pos = (0,0)):
	points = []
	for y in range(h):
		for x in range(w):
			points.append(Point(addvec(pos,[x*dist,y*dist])))
	for i in range(len(points)):
		if i < w:
			points[i].addconnection(Anchor(addvec(points[i].pos,[0,-dist])))
		else:
			points[i].addconnection(points[i-w])
		if i%w != 0:
			points[i].addconnection(points[i-1])
	return points

zoom = 0.2
zoomspeed = 0.01
gravityaccl = 1000
option = 1#int(input("select an option either 1 or 2:"))-1	 
mouse = Anchor([0,0])
points = []
if option == 0:
	points.append(Point([0,0]))
	for i in range(10):
		points.append(Point([40*(i+1),0]))
	points[0].addconnection(mouse,10)

	for i in range(1,len(points)):
		points[i].addconnection(points[i-1])
if option == 1: points+=createsheet(30,20,20)

SS = (600,600)

offset = [0,0]
p.init()
screen = p.display.set_mode(SS)
clock = p.time.Clock()
font = p.font.Font('freesansbold.ttf', 25)
fps_render_counter = 0
dt = 1/60
fps_render = font.render(str(int(1/dt)),False,"red")
fps_render_rect = fps_render.get_rect()
fps_render_rect.topright = (SS[0],0)

lastmousepos = -1
running = True
while running:
	dt = clock.tick(60)/1000
	for event in p.event.get():
		if event.type == p.QUIT:
			running = False
	keys = p.key.get_pressed()
	mousepressed = p.mouse.get_pressed()
	
	if keys[p.K_UP] and zoom < 1:
		zoom+=zoomspeed
	if keys[p.K_DOWN] and zoom > zoomspeed*2:
		zoom-=zoomspeed
	
	if mousepressed[0]:
		mousepos = p.mouse.get_pos()
		if lastmousepos != -1:
			offset[0]+= mousepos[0]-lastmousepos[0]
			offset[1]+= mousepos[1]-lastmousepos[1]
		lastmousepos = mousepos
	else:
		lastmousepos = -1
	if mousepressed[2]:
		for point in points:
			point.applyForce(setmag(point.vectorto(mouse),-1000))
			#point.applyForce(mvec(point.vectorto(mouse),-10))

	mouse.updatePos(mvec(addvec(p.mouse.get_pos(),mvec(offset,-1)),1/zoom))
	for point in points: point.update()
	for point in points: point.updatePos()
	fps_render_counter+=1
	if fps_render_counter>10:
		fps_render = font.render(str(int(1/dt)),False,"red")
		fps_render_rect = fps_render.get_rect()
		fps_render_rect.topright = (SS[0],0)
		fps_render_counter=0
	screen.fill("white")
	for point in points: point.draw()
	screen.blit(fps_render,fps_render_rect)
	
	p.display.update()