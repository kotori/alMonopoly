
#ifndef PLAYER_H
#define PLAYER_H

//
// MONOPOLY PLAYER CLASS
//
class MonopolyPlayer {
public:
    MonopolyPlayer();
    ~MonopolyPlayer();

    int get_id();
    void set_id(int id);

    int get_money();
    void set_money(int money);

    int get_score();
    void set_score(int score);

    int get_location();
    void set_location(int location);

    int get_x();
    void set_x(int x);

    int get_y();
    void set_y(int y);

    int get_animationX();
    void set_animationX(int sourceX);

    int get_animationY();
    void set_animationY(int sourceY);

    ALLEGRO_BITMAP* get_image();
    void set_image(ALLEGRO_BITMAP *image);

    Direction get_direction();
    void set_direction(Direction dir);

private:
    ALLEGRO_BITMAP *_image;
    Direction _dir;
    int _id;		/*!< Player's unique ID. */
    int _money;		/*!< Player's accumulated money. */
    int _score;		/*!< Player's score. */
    int _location;	/*!< Player's current location on the gameboard. */
    int _x;
    int _y;
    int _sourceX;
    int _sourceY;
};

#endif // PLAYER_H
