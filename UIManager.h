#pragma once
//진짜 많이 배워갑니다...

//상속을 막는 final
class UIManager final
{
	//
	static UIManager& Get()
	{
		static UIManager Instance;
		return Instance;
	}
	
	//복사 연산자와 복사 대입 연산자 삭제
	UIManager(const UIManager&) = delete;
	UIManager& operator=(const UIManager&) = delete;

	//이동 연산자와 이동 대입연산자 삭제
	UIManager(UIManager&&) = delete;
	UIManager& operator=(UIManager&&) = delete;

	//생성자 소멸자 정의하지않고 private로 차단
private:
	UIManager() = default;
	~UIManager() = default;


};