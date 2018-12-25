#include <iostream>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <wiringPi.h>

class LED8 
{
	public:
	LED8();						//构造函数
	~LED8();					//析构函数
	void putLED(uint8_t data);	//设置LED的状态
	void cleanLED(void);		//所有灯灭
	
	private:
	void _init(void);			//初始化IO
	void _update(void);			//更新IO
	bool leds[8];
};

LED8::LED8():leds{0}
{
	std::cout << "Debug: In LED8" << std::endl;
	wiringPiSetup();//初始化wiring
	_init();		//初始化IO
	cleanLED();		//灭灯
}

LED8::~LED8()
{
	std::cout << "Debug: In ~LED8" << std::endl;
	cleanLED();		//灭灯
}

void LED8::_init(void)
{
	pinMode(0, OUTPUT);
	pinMode(1, OUTPUT);
	pinMode(2, OUTPUT);
	pinMode(3, OUTPUT);
	pinMode(4, OUTPUT);
	pinMode(5, OUTPUT);
	pinMode(6, OUTPUT);
	pinMode(7, OUTPUT);
}

void LED8::putLED(uint8_t data)
{
	uint8_t i;
	
	for(i = 0; i < 8; i++)
	{
		leds[i] = ((data >> i) & 0x01);
	}
	_update();
}

void LED8::cleanLED(void)
{
	uint8_t i;
	
	for(i = 0; i < 8; i++)
	{
		leds[i] = 0;
	}
	_update();
}

void LED8::_update(void)
{
	uint8_t i;
	
	for(i = 0; i < 8; i++)
	{
		if(leds[i] == 1)
			digitalWrite(i, HIGH);
		else
			digitalWrite(i, LOW);
	}
}

int main(void)
{
	LED8 led1_8;			//创建8位LED对象
	
	char inputstr[1000]{0};		//输入缓冲区

	while(1)
	{
		std::cout << "->" ;
		std::cin >> inputstr;	//获取输入信息
		int len = strlen(inputstr);
		int i = 0;
		for(i = 0; i < len; i++)
		{
			if(!isdigit(inputstr[i]))
			{
				break;
			}
		}
		if(i >= len)	//是纯数字
		{
			int data = atoi(inputstr);
			if((data > 255) || (data < 0))	//超出8位，报错
			{
				std::cout << "Input error!" << std::endl;
			}
			else
			{
				led1_8.putLED((uint8_t)atoi(inputstr));
			}
		}
		else		//是其他字符串
		{
			if(strcmp(inputstr, "clean") == 0)
			{
				led1_8.putLED(0x00);
			}
			else if(strcmp(inputstr, "all") == 0)
			{
				led1_8.putLED(0xFF);
			}
			else  if(strcmp(inputstr, "water") == 0)
			{
				led1_8.putLED(0x00);
				delay(100);
				for(int8_t i{0}; i < 8; i++)
				{
					led1_8.putLED((uint8_t)pow(2, i));
					delay(100);
				}
				for(int8_t i{7}; i >= 0; i--)
				{
					led1_8.putLED((uint8_t)pow(2, i));
					delay(100);
				}
				led1_8.putLED(0x00);
			}
			else if(strcmp(inputstr, "help") == 0)
			{
				std::cout \
					<<"输入一个小于256大于等于0的数字，以显示到8位LED。\n" \
					<<"输入help，获取帮助信息。\n" \
					<<"输入all，以点亮所有LED。\n" \
					<<"输入clean，以熄灭所有LED。\n" \
					<<"输入water，以显示一次流水灯。\n" \
					<<"输入exit，退出。" \
					<<std::endl;
			}
			else if(strcmp(inputstr, "exit") == 0)
			{
				led1_8.putLED(0);
				return 0;
			}
			else
			{
				std::cout << "Input error!" << std::endl;
			}
		}
	}

	return 0;
}
