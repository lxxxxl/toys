import pygame
import random
from enum import Enum, IntEnum

# Screen width
WIDTH = 800
# Screen height
HEIGHT = 600
# Game FPS
FPS = 100
# Size of ant
ANT_SIZE = 20
# Ants count
ANTS_COUNT = 20

class Color(Enum):
    """Allowed colors"""
    WHITE   = (255, 255, 255)
    BLACK   = (0, 0, 0)
    RED     = (255, 0, 0)
    GREEN   = (0, 255, 0)
    BLUE    = (0, 0, 255)
    PINK    = (255, 0, 255)
    YELLOW  = (255, 255, 0)
    CYAN    = (0, 255, 255)

class Direction(IntEnum):
    """Ant direction"""
    UP      = 1
    RIGHT    = 2
    DOWN    = 3
    LEFT   = 4

    def next(self):
        """Rotate right"""
        if self.value == self.LEFT:
            return self.UP
        else:
            return Direction(self.value + 1)

    def previous(self):
        """Rotate left"""
        if self.value == self.UP:
            return self.LEFT
        else:
            return Direction(self.value - 1)

class Step(pygame.sprite.Sprite):
    """Ant step"""
    def __init__(self, color, size, x, y):
        """Class constructor"""
        pygame.sprite.Sprite.__init__(self)
        self.size = size
        self.color = color
        self.image = pygame.Surface((size, size))
        self.image.fill(color.value)
        self.rect = self.image.get_rect()
        self.rect.center = (x+size/2, y+size/2)

class Ant(pygame.sprite.Sprite):
    """Ant class"""
    def __init__(self, color, size, x ,y):
        """Class constructor"""
        pygame.sprite.Sprite.__init__(self)
        self.size = size
        self.color = color
        self.image = pygame.Surface((size, size))
        self.image.fill(self.color.value)
        self.rect = self.image.get_rect()
        self.rect.center = (x+size/2, y+size/2)
        self.direction = random.choice(list(Direction))

    def steps(self, steps):
        """Set steps group"""
        self.steps = steps

    def make_step(self):
        """Place step in current position"""
        step = Step(self.color, self.size, self.rect.x, self.rect.y)
        self.steps.add(step)

    def update(self):
        """Move to desired direction"""
        if self.direction == Direction.UP:
            self.rect.y -= self.size
        elif self.direction == Direction.DOWN:
            self.rect.y += self.size
        elif self.direction == Direction.RIGHT:
            self.rect.x += self.size
        elif self.direction == Direction.LEFT:
            self.rect.x -= self.size

        # Process colors
        step_color = Color.WHITE
        for step in self.steps:
            if self.rect.center == step.rect.center:
                step_color = step.color
                step.kill()

        if step_color == self.color:
            self.direction = self.direction.previous()
        elif step_color == Color.WHITE:
            self.make_step()
            self.direction = self.direction.next()
        else:
            self.direction = self.direction.next().next()

        # Process screen borders
        if self.rect.left > WIDTH:
            self.rect.right = 0 + self.size
        elif self.rect.right < 0:
            self.rect.left = WIDTH - self.size
        elif self.rect.top > HEIGHT:
            self.rect.bottom = 0 + self.size
        elif self.rect.bottom < 0:
            self.rect.top = HEIGHT - self.size



# Init game screen
pygame.init()
pygame.mixer.init()
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Langton ant")
clock = pygame.time.Clock()

# Init sprite groups
steps = pygame.sprite.Group()
ants = pygame.sprite.Group()

# Init ants
for i in range(0, ANTS_COUNT):
    colors = list([Color.BLACK, Color.RED, Color.GREEN, Color.BLUE, Color.YELLOW, Color.PINK, Color.CYAN])
    ant = Ant(random.choice(colors),
                ANT_SIZE, 
                random.choice(range(0, WIDTH, ANT_SIZE)), 
                random.choice(range(0, HEIGHT, ANT_SIZE)))
    ant.steps(steps)
    ants.add(ant)

# Game cycle
running = True
while running:
    # Work on fixed FPS
    clock.tick(FPS)
    # Process events
    for event in pygame.event.get():
        # Check for closing window
        if event.type == pygame.QUIT:
            running = False

    # Update sprites
    ants.update()
    steps.update()

    # Render data
    screen.fill(Color.WHITE.value)
    ants.draw(screen)
    steps.draw(screen)
    # Flip frame
    pygame.display.flip()

pygame.quit()