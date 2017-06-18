#include <cmath>

void gather_mouse(int &x, int &y) {
    /* WINDOWS SHOULD BE LIKE:
     * 
     * #include <Windef.h>
     * POINT p;
     * GetCursorPos(&p);
     * x = p.x;
     * y = p.y;
    */
    char buf[256];
    FILE *bash = popen("eval $(xdotool getmouselocation --shell); echo $X $Y", "r");
    fgets(buf, sizeof(buf), bash);
    pclose(bash);
    sscanf(buf, "%d %d", &x, &y);
}

class Actor {
public:
    static constexpr int kDirectionThreshold = 300;

    struct Point {
        int x, y;
    };

    enum Direction {
        None, Up, Right, Down, Left
    };

    Actor() : id_() {}
    Actor(int id) : id_(id) {}

    virtual std::string name() {
        return "Actor" + std::to_string(id_);
    }

    virtual void tick(uint64_t delta) {
        //printf("[%lu]Tick %s ", delta, name().c_str());
        int x, y;
        gather_mouse(x, y);
        //printf("%d %d\n", x, y);

        Direction new_direction = get_direction(Point{x, y});
        if (new_direction != None) {
            printf("player.setSwingState(%s)\n", dir_str(new_direction));
            ref_position_ = {x, y};
            direction_ = new_direction;
        }
    }

    Direction get_direction(Point cur_position) {
        Direction ret = None;
        int delta_x = cur_position.x - ref_position_.x;
        int delta_y = cur_position.y - ref_position_.y;
        if (abs(delta_x) >= kDirectionThreshold) {
            ret = (delta_x > 0) ? Right : Left;
        }
        if (abs(delta_y) >= kDirectionThreshold && abs(delta_y) > abs(delta_x)) {
            return (delta_y > 0) ? Down : Up;
        }
        
        return ret;
    }
// 
protected:
    const char *dir_str(Direction dir) {
        static const char *strs[] = {
            "None", "Up", "Right", "Down", "Left"
        };
        return strs[(int)dir];
    }

    Direction direction_;
    Point ref_position_;
    int id_;
};
