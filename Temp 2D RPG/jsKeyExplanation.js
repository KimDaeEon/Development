#pragma strict

static var hit = 0;

static var lose = 0;
private var w: int;
private var h: int;
private var rect: Rect;

function Start(){
	hit = 0;
	lose = 0;
}

function Update(){
	if(Input.GetAxis("Cancel")){
		Application.Quit();
	}
}

function OnGUI(){
	w = Screen.width;
	h = Screen.height;
	GUI.Label(Rect(0.9*w,0 ,w,h*0.03f), "<color=#000000>방향키 : 상,하,좌,우 이동</color>");
	GUI.Label(Rect(0.9*w, 40 ,w,h*0.03f),"<color=#000000>z : 일반 공격</color>");
	GUI.Label(Rect(0.9*w, 80 ,w,h*0.03f),"<color=#000000>x : 크게 휘두르기</color>");
	GUI.Label(Rect(0.9*w, 120 ,w,h*0.03f),"<color=#000000>ESC : 게임 종료</color>");
	}