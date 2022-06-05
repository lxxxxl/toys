import random
from enum import Enum, IntEnum
from tkinter import Tk, Canvas

# Screen width
WIDTH = 800
# Screen height
HEIGHT = 600
# Game FPS
FPS = 60
# Size of entities
CELL_SIZE = 16
#count of cells
CELLS_HEIGHT = int(HEIGHT/CELL_SIZE)
CELLS_WIDTH = int(WIDTH/CELL_SIZE)
# Victims count
VICTIMS_COUNT = 5
# hardcode deltas for neighbor cells because itertools.product is too slow
NEIGHBOR_CELLS = [(-1, -1), (-1, 0), (-1, 1), (0, -1), (0, 1), (1, -1), (1, 0), (1, 1)]

CELLS = [[0 for x in range(CELLS_HEIGHT)] for y in range(CELLS_WIDTH)]

def rgbtohex(r,g,b):
    return f'#{r:02x}{g:02x}{b:02x}'

class Creature:
    """Base class for all creatures"""
    def __init__(self, x, y):
        self.x = x
        self.y = y
        self.color = [0, 0, 0]
        self.age = 0
        CELLS[self.x][self.y] = self

    def get_color(self):
        """Get HEX representation of color"""
        return f'#{self.color[0]:02x}{self.color[1]:02x}{self.color[2]:02x}'

    def normalize_coords(self, dx, dy):
        """Normalize neighbor coords"""
        check_x = self.x+dx
        check_y = self.y+dy

        if check_x >= CELLS_WIDTH:
            check_x -= CELLS_WIDTH
        elif check_x < 0:
            check_x += CELLS_WIDTH

        if check_y >= CELLS_HEIGHT:
            check_y -= CELLS_HEIGHT
        elif check_y < 0:
            check_y += CELLS_HEIGHT

        return (check_x, check_y)


class Grass(Creature):
    """Grass class"""
    def __init__(self, x, y):
        """Class constructor"""
        Creature.__init__(self, x, y)
        self.color = [0, 255, 0]

    @classmethod
    def create(cls, x, y):
        """Constructor wrapper"""
        # die if cell allready occupied
        if CELLS[x][y] != 0:
            return None
        return cls(x, y)

    def update(self):
        """Update grass state"""
        free_cells = []
        for dx, dy in NEIGHBOR_CELLS:
            (check_x, check_y) = self.normalize_coords(dx, dy)
            
            if CELLS[check_x][check_y] == 0:
                free_cells.append((check_x, check_y))

        if self.age >= 2 and len(free_cells) > 0:
            for cell in free_cells:
                if random.choice([True, False, False]):
                    continue
                # create child
                CELLS[cell[0]][cell[1]] = Grass.create(cell[0], cell[1])

        self.age += 1
            

class Victim(Creature):
    """Victim class"""
    def __init__(self, x, y, hungry):
        """Class constructor"""
        Creature.__init__(self, x, y)
        self.color = [0, 0, 255]
        self.hungry = hungry
    
    @classmethod
    def create(cls, x, y):
        """Constructor wrapper"""
        # die if cell allready occupied or eat if there is grass
        hungry = True
        if CELLS[x][y] != 0:
            if isinstance(CELLS[x][y], Grass):
                hungry = False
            else:
                return 0

        return cls(x, y, hungry)

    def set_victims_group(self, victims):
        """Set victim's group to add children to drawable group"""
        self.victims = victims

    def update(self):
        """Update victim state"""
        # die if too old
        if self.age > 5:
            CELLS[self.x][self.y] = 0
            return

        # count victims around
        free_cells = []
        for dx, dy in NEIGHBOR_CELLS:
            (check_x, check_y) = self.normalize_coords(dx, dy)

            if CELLS[check_x][check_y] == 0:
                free_cells.append((check_x, check_y))
            elif isinstance(CELLS[check_x][check_y], Grass):
                if self.hungry:
                    # eat grass
                    CELLS[check_x][check_y] = 0
                    self.hungry = False
                free_cells.append((check_x, check_y))

        #populate
        if len(free_cells) > 3 and not self.hungry:
            # get random free cell near current cell
            cell = random.choice(free_cells)
            # create child
            CELLS[cell[0]][cell[1]] = Victim.create(cell[0], cell[1])
            self.hungry = True

        # set new color in shades of blue
        self.color[2] -= 30
        self.age += 1


class Predator(Creature):
    """Predator class"""
    def __init__(self, x, y, hungry):
        """Class constructor"""
        Creature.__init__(self, x, y)
        self.color = [255, 0, 0]
        self.hungry = hungry


    @classmethod
    def create(cls, x, y):
        """Constructor wrapper"""
        # die if cell allready occupied or eat if there is victim
        hungry = True
        if CELLS[x][y] != 0:
            if isinstance(CELLS[x][y], Victim):
                hungry = False
            elif isinstance(CELLS[x][y], Grass):
                pass
            else:
                return 0

        return cls(x, y, hungry)

    def update(self):
        """Update predator state"""
        # die if too old
        if self.age > 5:
            CELLS[self.x][self.y] = 0

        # count victims around
        free_cells = []
        for dx, dy in NEIGHBOR_CELLS:
            (check_x, check_y) = self.normalize_coords(dx, dy)

            if CELLS[check_x][check_y] == 0:
                free_cells.append((check_x, check_y))
            else:
                if isinstance(CELLS[check_x][check_y], Victim):
                    # eat
                    if self.hungry:
                        CELLS[check_x][check_y] = 0
                        self.hungry = False
                        free_cells.append((check_x, check_y))
                elif isinstance(CELLS[check_x][check_y], Predator):
                    # old predators will eat other predators
                    if self.age > 3 and self.hungry:
                        CELLS[check_x][check_y] = 0
                        self.hungry = False
                        free_cells.append((check_x, check_y))

        #populate
        if len(free_cells) > 1  and not self.hungry:
            # get random free cell near current cell
            cell = random.choice(free_cells)
            # create child
            CELLS[cell[0]][cell[1]] = Predator.create(cell[0], cell[1])
            self.hungry = True

        self.age += 1
        # set new color in shades of red
        self.color[0] -= 30


# Init game screen
tk = Tk()
tk.title = 'Predator - Victim'
tk.resizable(0,0)

canvas = Canvas(tk, width=WIDTH, height=HEIGHT, bd=0, highlightthickness=0)
canvas.pack()

# create grass
for x in range(0, CELLS_WIDTH):
    for y in range(0, CELLS_HEIGHT):
        Grass.create(x, y)


# Init victims
for i in range(0, VICTIMS_COUNT):
    Victim.create(random.randrange(CELLS_WIDTH-1), random.randrange(CELLS_HEIGHT-1))

# Game cycle
running = True
frameno = 0
while running:
    tk.update_idletasks()
    tk.update()

    canvas.delete("all")

    count_grass = 0
    count_victims = 0
    count_predators = 0

    for x in range(0, CELLS_WIDTH):
        for y in range(0, CELLS_HEIGHT):
            if CELLS[x][y] == 0:
                continue

            if isinstance(CELLS[x][y], Grass):       count_grass += 1
            elif isinstance(CELLS[x][y], Victim):    count_victims += 1
            if isinstance(CELLS[x][y], Predator):    count_predators += 1
            
            canvas.create_rectangle(CELL_SIZE*x, CELL_SIZE*y,
                               CELL_SIZE*x+CELL_SIZE, CELL_SIZE*y+CELL_SIZE,
                               fill=CELLS[x][y].get_color(),
                               outline=CELLS[x][y].get_color())
            CELLS[x][y].update()

    # create predator on random
    if count_predators < 10 and random.choice([True, False, False, False]):
        Predator.create(random.randrange(CELLS_WIDTH-1), random.randrange(CELLS_HEIGHT-1))

    # grow grass if there is too few
    if count_grass < (CELLS_HEIGHT*CELLS_WIDTH/2):
        Grass.create(random.randrange(CELLS_WIDTH-1), random.randrange(CELLS_HEIGHT-1))

    if count_victims == 0:
        print(f'All dead in {frameno} frames')
        running = False

    frameno +=1