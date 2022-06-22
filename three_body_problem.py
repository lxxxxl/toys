from Box2D import *
from Box2D.examples.framework import Framework


class Simulation(Framework):
    def __init__(self):
        super(Simulation, self).__init__()
        # Disable default gravity
        self.world.gravity = (0.0, 0.0)
        # Gravity constant
        self.G = 100

        # Body 1
        circle_small = b2FixtureDef(shape=b2CircleShape(radius=0.1), density=1, friction=0.5, restitution=0.2)
        self.world.CreateBody(type=b2_dynamicBody, position=b2Vec2(0, 2), fixtures=circle_small, linearVelocity=(10, 0))

        # Body 2
        circle_small = b2FixtureDef(shape=b2CircleShape(radius=0.5), density=1, friction=0.5, restitution=0.2)
        self.world.CreateBody(type=b2_dynamicBody, position=b2Vec2(0, 5), fixtures=circle_small, linearVelocity=(20, 10))

        # Body 3
        circle_small = b2FixtureDef(shape=b2CircleShape(radius=1.0), density=1, friction=0.5, restitution=0.2)
        self.world.CreateBody(type=b2_dynamicBody, position=b2Vec2(0, 15), fixtures=circle_small, linearVelocity=(10, 20))


        # World frames 
        #TODO remove frames
        self.world.CreateBody(shapes=b2LoopShape(vertices=[(20, 0), (20, 40), (-20, 40), (-20, 0)]))

    def Step(self, settings):
        super(Simulation, self).Step(settings)

        # Simulate the Newton's gravity
        for bi in self.world.bodies:
            for bk in self.world.bodies:
                if bi == bk:
                    continue

                pi, pk = bi.worldCenter, bk.worldCenter
                mi, mk = bi.mass, bk.mass
                delta = pk - pi
                r = delta.length
                if abs(r) < 1.0:
                    r = 1.0

                force = self.G * mi * mk / (r * r)
                delta.Normalize()
                bi.ApplyForce(force * delta, pi, True)

if __name__ == "__main__":
    Simulation().run()