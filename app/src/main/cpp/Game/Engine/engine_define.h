//
// Created by 葵ユメ on 2018/08/25.
//


#ifndef AOIYUME_ENGINE_DEFINE_H
#define AOIYUME_ENGINE_DEFINE_H

	enum TouchEvent{
		eTouchEvent_DOWN,
		eTouchEvent_UP,
		eTouchEvent_MOVE,
		eTouchEvent_CANCEL,
		eTouchEvent_OUTSIDE,
		eTouchEvent_HOVER_MOVE = 7,
		eTouchEvent_HOVER_ENTER = 9,
		eTouchEvent_BUTTON_PRESS = 11,
		eTouchEvent_BUTTON_RELEASE
	};

	#define NET_CONNECT_MAX (4)

#endif