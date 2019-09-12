#pragma once
#include <Siv3D.hpp>

class SE {
public:
	SE(const FilePath& path);
	~SE();
	void play();//SE���Đ�����B�d�����čĐ�����悤�Ƃ������A�ŏ�����Đ����Ȃ���
private:
	Audio* sound;
};

class MapFlip {	//�n���ꂽ�傫���摜����؂�o�����摜��Ԃ�
public:
	MapFlip(Texture map, int xFlipWidth, int yFlipHeight);
	bool nextFlip();			//���̐؂�o���摜�̈ʒu�ɃZ�b�g�B�����Ȃ�(�Ō�)�Ȃ�false��Ԃ�
	TextureRegion getFlip();	//�؂�o���摜��Ԃ�
private:
	Texture map;	//�傫���摜
	int xMapWidth, yMapHeight;		//�傫���摜�̑傫��
	int xFlipWidth, yFlipHeight;	//�؂�o���T�C�Y
	int xNowPos, yNowPos;			//���݂̉摜�؂�o���ʒu
};

struct FlipMovie :IEffect {//MapFlip�𗘗p���ăp���p����������G�t�F�N�g
	FlipMovie(Texture map, int xFlipWidth, int yFlipHeight, int xDraw, int yDraw);
	~FlipMovie();
	bool update(double t);
private:
	MapFlip* mapflip;
	int xDraw, yDraw;			//�`��ʒu
	double switchTime;	//�Ō�ɐ؂�o���摜��؂�ւ�������
};

class FlipEffect {//����FlipMovie�G�t�F�N�g���J��Ԃ��Đ�����ۂ̃��\�[�X��ێ�����
public:
	FlipEffect(const FilePath& path, int xFlipWidth, int yFlipHeight, int xDraw, int yDraw);
	void setTexture(const FilePath& path, int xFlipWidth, int yFlipHeight);	//�摜���Z�b�g("�摜�p�X",�؂�o���T�C�Y)
	void setPos(int xDraw, int yDraw);	//�`��ʒu���Z�b�g
	void play();						//�Đ��J�n
	void play(int xDraw, int yDraw);	//�Đ��J�n(�`��ʒu)
	void draw();	//�`��(�t���[�����ɌĂяo���K�v����)
private:
	Effect effect;
	Texture map;
	int xFlipWidth, yFlipHeight;	//�؂�o���T�C�Y
	int xDraw, yDraw;				//�`��ʒu
};

/*
struct Fractal :IEffect {//�t���N�^���͗l�����o���G�t�F�N�g
	Fractal(int size, int x, int y);
	bool update(double t);
private:
	int xFirst, yFirst;
	int firstSize;
	int x2, y2;

	void drawTriangle(double t, int xPos, int yPos, int size, int x2, int y2);
};
*/

