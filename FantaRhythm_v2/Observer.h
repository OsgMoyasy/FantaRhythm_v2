#pragma once
#include <list>


enum Massage {//イベント名一覧　そこまで増えそうにないので一括管理
	NONE,//ない場合
	UPATTACK,//上ボタン攻撃
	DOWNATTACK,//下ボタン攻撃
	BOTHATTACK,//上下ボタン攻撃
	DAMAGE,		//小ダメージ
	CRITICALDAMAGE,//大ダメージ
};

typedef struct _Obj {//イベント用オブジェクト　
	Massage msg;//イベント名
	int val;
}Obj;

/*--------------------------------------------------------
	クラス名：Observer
	機能：観察者

---------------------------------------------------------*/
class Observer{
public:
	//virtual ~Observer(void); 
	virtual void funcEvent(Obj obj) = 0;

	void setObject(class Subject* pSubject){//呼び出されるハンドラ Subject側から呼び出される
		m_pSubject = pSubject;
	}
protected://派生先のprivateメンバになるprotected
	class Subject* m_pSubject;

};

/*--------------------------------------------------------
	クラス名：Subject
	機能：観察対象 通知する側
		  オブザーバのリストを保持
		　自分に状態の変化があると保持しているオブザーバ達に通知を送る


-----------------------------------------------------------*/
class Subject {
public:
	//Observer全て削除
	virtual ~Subject(void) {
		std::list<Observer*>::iterator it;
		for (it = m_observers.begin(); it != m_observers.end(); it++) {
			(*it)->setObject(NULL);
		}
	}
	//登録されたObserverのハンドラを呼び出す
	virtual void notifyObservers(void) {
		std::list<Observer*>::iterator it;
		for (it = m_observers.begin(); it != m_observers.end(); it++) {
			(*it)->funcEvent(getMassage());
		}
	}
	//Observerを登録
	virtual void addObserver(class Observer* pObserver) {
		m_observers.push_back(pObserver);
		pObserver->setObject(this);
	}
	virtual Obj getMassage() = 0;


protected:
	std::list<class Observer*> m_observers;//オブザーバを保持
};

class NotesSubject : public Subject {
private:
	Obj o;
public:
	//イベント用のメッセージを返す
	Obj getMassage() {
		return o;
	}
	//イベントをセットする
	void setEvent(Massage msg, int lane) {
		o = {msg,lane};
	}
};

class CharacterSubject : public Subject {
private:
	Obj o;
public:
	//イベント用のメッセージを返す
	Obj getMassage() {
		return o;
	}
	//イベントをセットする
	void setEvent(int attack) {
		o = { NONE, attack };
	}
};
