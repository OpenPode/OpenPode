/*
 * PS4Controller.cpp
 *
 *  Created on: 23 nov. 2015
 *      Author: Julien
 */

#include "PS4Controller.h"

const std::string PS4Controller::m_device_ev("/dev/input/event0");
const std::string PS4Controller::m_device_js("/dev/input/js0");

PS4Controller::PS4Controller() :
		m_is_cross_press(false), m_is_square_press(false),
		m_is_triangle_press(false), m_is_circle_press(false),

		m_is_r1_press(false), m_is_r2_press(false), m_is_r3_press(false),
		m_is_l1_press(false), m_is_l2_press(false), m_is_l3_press(false),

		m_is_ps_press(false), m_is_options_press(false), m_is_share_press(false),

		m_jsl_x_value(0), m_jsl_y_value(0),
		m_jsr_x_value(0), m_jsr_y_value(0)
{
	m_fd_ev = fdopen(m_device_ev, O_NONBLOCK);
	m_fd_js = fdopen(m_device_js, O_NONBLOCK);

}

void PS4Controller::process_input()
{
	m_rd_ev = read( m_fd_ev, &m_ev, sizeof(input_event) );
	m_rd_js = read( m_fd_js, &m_js, sizeof(js_event) );

	if( m_rd_ev < sizeof(input_event) )
	{
		printf("Error reading %s\n", m_device_ev.c_str());
		exit(-1);
	}
	if( m_rd_js < sizeof(js_event) )
	{
		printf("Error reading %s\n", m_device_js.c_str());
		exit(-1);
	}

	if( m_ev.code == KEY_CROSS )
		m_is_cross_press = not m_is_cross_press;

	if( m_ev.code == KEY_CIRCLE )
		m_is_circle_press = not m_is_circle_press;

	if( m_ev.code == KEY_SQUARE )
		m_is_square_press = not m_is_square_press;

	if( m_ev.code == KEY_TRIANGLE )
		m_is_triangle_press = not m_is_triangle_press;

	/*if( m_ev.code == KEY_DPAD_UP and m_ev.value == -1 )

	if( m_ev.code == KEY_DPAD_DOWN and m_ev.value == 1 )

	if( m_ev.code == KEY_DPAD_LEFT and m_ev.value == -1 )

	if( m_ev.code == KEY_DPAD_RIGHT and m_ev.value == 1 )*/

	if( m_ev.code == KEY_R1 )
		m_is_r1_press = not m_is_r1_press;

	if( m_ev.code == KEY_R2 )
		m_is_r2_press = not m_is_r2_press;

	if( m_ev.code == KEY_R3 )
		m_is_r3_press = not m_is_r3_press;

	if( m_ev.code == KEY_L1 )
		m_is_l1_press = not m_is_l1_press;

	if( m_ev.code == KEY_L2 )
		m_is_l2_press = not m_is_l2_press;

	if( m_ev.code == KEY_L3 )
		m_is_l3_press = not m_is_l3_press;

	if( m_ev.code == KEY_PS )
		m_is_ps_press = not m_is_ps_press;

	if( m_ev.code == KEY_OPTIONS )
		m_is_options_press = not m_is_options_press;

	if( m_ev.code == KEY_SHARE )
		m_is_share_press = not m_is_share_press;

	if( m_js.type == JS_EVENT_AXIS and m_js.number == 0 )
		m_jsl_x_value = -m_js.value;

	if( m_js.type == JS_EVENT_AXIS and m_js.number == 1 )
		m_jsl_y_value = -m_js.value;

	if( m_js.type == JS_EVENT_AXIS and m_js.number == 2 )
		m_jsr_x_value = -m_js.value;

	if( m_js.type == JS_EVENT_AXIS and m_js.number == 5 )
		m_jsr_y_value = -m_js.value;
}

int PS4Controller::fdopen(std::string device, int mode)
{
    int fd = open(device.c_str(), mode);

    if( fd == -1)
    {
        printf("Unable to open file : %s\n", device.c_str());
        exit(-1);
    }

    return fd;
}



/*

enum Key_event_type
{
    Key_release,
    Key_press,
    Key_autorepeat
};

int open(std::string device, int mode)
{
    int fd = open(device.c_str(), mode);

    if( fd == -1)
    {
        printf("Unable to open file : %s\n", device.c_str());
        exit(-1);
    }

    return fd;
}
*/
