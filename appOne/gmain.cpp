
#include"libOne.h"
struct HAND {
	int hand;
	int img[3];
	float px, py, angle;
	float r, g, b;
	int life;
};

struct DATA {
	int INIT = 0;
	int PLAY = 1;
	int RESULT = 2;
	int state = INIT;

	int GU = 0;
	int CHOKI = 1;
	int PA = 2;
	struct HAND player;
	struct HAND pc;
	int heartImg;

	float resultSize;
};

void loadImages(struct DATA* d) {
	d->player.img[0] = loadImage("assets\\playerGu.png");
	d->player.img[1] = loadImage("assets\\playerChoki.png");
	d->player.img[2] = loadImage("assets\\playerPa.png");
	d->pc.img[0] = loadImage("assets\\playerGu.png");
	d->pc.img[1] = loadImage("assets\\playerChoki.png");
	d->pc.img[2] = loadImage("assets\\playerPa.png");
	d->heartImg = loadImage("assets\\heart.png");
}
void init(struct DATA* d) {
	d->player.hand = d->GU;
	d->player.px = 250;
	d->player.py = 225;
	d->player.angle = 0;
	d->player.r = 255;
	d->player.g = 255;
	d->player.b = 255;
	d->player.life = 3;

	d->pc.hand = d->GU;
	d->pc.px = 800 - 250;
	d->pc.py = 225;
	d->pc.angle = 0;
	d->pc.r = 255;
	d->pc.g = 255;
	d->pc.b = 255;
	d->pc.life = 3;

	d->resultSize = 0;

	d->state = d->PLAY;
}
void play(struct DATA* d) {
	if (!isTrigger(KEY_Q) && !isTrigger(KEY_W) && !isTrigger(KEY_E)) {
		return;
	}
	//プレイヤーの手
	if (isTrigger(KEY_Q)) { d->player.hand = d->GU; }
	if (isTrigger(KEY_W)) { d->player.hand = d->CHOKI; }
	if (isTrigger(KEY_E)) { d->player.hand = d->PA; }
	//PCの手
	d->pc.hand = random() % 3;
	//勝ち負け判定
	if (d->player.hand == d->pc.hand) {
		//あいこ
		d->player.r = 255;  d->player.g = 255;  d->player.b = 255;
		d->pc.r = 255;  d->pc.g = 255;  d->pc.b = 255;
	}
	else if ((d->player.hand + 1) % 3 == d->pc.hand) {
		//プレイヤーの勝ち
		d->pc.life--;
		d->player.r = 255;  d->player.g = 200;  d->player.b = 200;
		d->pc.r = 255;  d->pc.g = 255;  d->pc.b = 255;
	}
	else {
		//ｐｃの勝ち
		d->player.life--;
		d->player.r = 255;  d->player.g = 255;  d->player.b = 255;
		d->pc.r = 255;  d->pc.g = 200;  d->pc.b = 200;
	}
	if (d->player.life == 0 || d->pc.life == 0) {
		d->state = d->RESULT;
	}
}
void result(struct DATA* d) {
	if (d->player.life == 0) {
		d->player.py += 1;
		d->player.angle += 0.003f;
	}
	else {
		d->pc.py += 1;
		d->pc.angle += -0.003f;
	}
	if (d->resultSize < 180) {
		d->resultSize += 10;
	}
	if (isTrigger(KEY_SPACE)) {
		d->state = d->INIT;
	}
}

void draw(struct DATA* d) {
	clear(180);
	rectMode(CENTER);

	imageColor(d->player.r, d->player.g, d->player.b);
	image(d->player.img[d->player.hand],
		d->player.px, d->player.py, d->player.angle);

	imageColor(d->pc.r, d->pc.g, d->pc.b);
	image(d->pc.img[d->pc.hand],
		d->pc.px, d->pc.py, d->pc.angle);

	imageColor(255, 0, 0);
	for (int i = 0; i < d->player.life; i++) {
		image(d->heartImg, d->player.px + 50 * (i - 1), d->player.py - 110);
	}
	for (int i = 0; i < d->pc.life; i++) {
		image(d->heartImg, d->pc.px + 50 * (i - 1), d->pc.py - 110);
	}


	if (d->state == d->RESULT) {
		textSize(d->resultSize);
		if (d->player.life > 0) {
			fill(255, 0, 0);
			text("勝ち", 225, 320);
		}
		else {
			fill(0, 0, 200);
			text("負け", 255, 320);
		}
	}
}

void gmain() {
	window(800, 450);
	struct DATA d;
	loadImages(&d);
	while (notQuit) {
		if (d.state == d.INIT) { init(&d); }
		else if (d.state == d.PLAY) { play(&d); }
		else if (d.state == d.RESULT) { result(&d); }
		draw(&d);
	}
}

