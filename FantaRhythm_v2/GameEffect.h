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
	FlipMovie(Texture map, int xFlipWidth, int yFlipHeight, int xDraw = 0, int yDraw = 0, double switchBetween = 0.03);
	~FlipMovie();
	bool update(double t);
private:
	MapFlip* mapflip;
	int xDraw, yDraw;			//�`��ʒu
	double switchTime;	//�Ō�ɐ؂�o���摜��؂�ւ�������
	double switchBetween;
};

class FlipEffect {//����FlipMovie�G�t�F�N�g���J��Ԃ��Đ�����ۂ̃��\�[�X��ێ�����BSE���Đ��ł���B
public:
	FlipEffect(const FilePath& path, int xFlipWidth, int yFlipHeight, int xDraw = 0, int yDraw = 0, double switchBetween = 0.03);
	~FlipEffect();
	void setTexture(const FilePath& path, int xFlipWidth, int yFlipHeight);	//�摜���Z�b�g("�摜�p�X",�؂�o���T�C�Y)
	void setPos(int xDraw, int yDraw);	//�`��ʒu���Z�b�g
	void setSE(FilePath& path);			//SE���Z�b�g
	void play();						//�Đ��J�n
	void play(int xDraw, int yDraw);	//�Đ��J�n(�`��ʒu)
	void draw();	//�`��(�t���[�����ɌĂяo���K�v����)
private:
	//�G�t�F�N�g�֌W
	Effect effect;
	Texture map;
	int xFlipWidth, yFlipHeight;	//�؂�o���T�C�Y
	int xDraw, yDraw;				//�`��ʒu
	double switchBetween;

	//SE�֌W
	SE* se;
	bool existSE;	//SE���ݒ肳��Ă��邩
};

template<typename T>
class FlipMap {//FlipEffect��unordered_map�ŊǗ�����ׂ̃N���X�B�L�[�Ɏg���^�͎��R�B
public:
	~FlipMap();
	void set(T name, const FilePath& path, int xFlipWidth, int yFlipHeight, int xDraw = 0, int yDraw = 0);	//�g�p����G�t�F�N�g��o�^
	FlipEffect* get(T name);	//�G�t�F�N�g���w�肵�Ď擾
	void draw();					//�Đ����̑S�ẴG�t�F�N�g��`��
private:
	std::unordered_map<T, FlipEffect*> usingEffect;
};
#include"FlipSet.h"

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

