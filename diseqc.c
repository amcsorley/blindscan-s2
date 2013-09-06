/* diseqc -- simple diseqc commands for the Linux DVB S2 API
 *
 * UDL (updatelee@gmail.com)
 * Derived from work by:
 * 	Igor M. Liplianin (liplianin@me.by)
 * 	Alex Betis <alex.betis@gmail.com>
 *	Pendragon
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "diseqc.h"

struct dvb_diseqc_master_cmd committed_switch_cmds[] = {
	{ { 0xE0, 0x10, 0x38, 0xF0, 0x00, 0x00 }, 4 },
	{ { 0xE0, 0x10, 0x38, 0xF4, 0x00, 0x00 }, 4 },
	{ { 0xE0, 0x10, 0x38, 0xF8, 0x00, 0x00 }, 4 },
	{ { 0xE0, 0x10, 0x38, 0xFC, 0x00, 0x00 }, 4 }
};

struct dvb_diseqc_master_cmd uncommitted_switch_cmds[] = {
	{ { 0xE0, 0x10, 0x39, 0xF0, 0x00, 0x00 }, 4 },
	{ { 0xE0, 0x10, 0x39, 0xF1, 0x00, 0x00 }, 4 },
	{ { 0xE0, 0x10, 0x39, 0xF2, 0x00, 0x00 }, 4 },
	{ { 0xE0, 0x10, 0x39, 0xF3, 0x00, 0x00 }, 4 },
	{ { 0xE0, 0x10, 0x39, 0xF4, 0x00, 0x00 }, 4 },
	{ { 0xE0, 0x10, 0x39, 0xF5, 0x00, 0x00 }, 4 },
	{ { 0xE0, 0x10, 0x39, 0xF6, 0x00, 0x00 }, 4 },
	{ { 0xE0, 0x10, 0x39, 0xF7, 0x00, 0x00 }, 4 }
};

struct dvb_diseqc_master_cmd dir_cmd[] =
{
	{ { 0xe0, 0x31, 0x68, 0xFF, 0x00, 0x00 }, 4 }, // Drive Motor East 1 step
	{ { 0xe0, 0x31, 0x69, 0xFF, 0x00, 0x00 }, 4 }  // Drive Motor West 1 step
};

double radian( double number )
{
	return number*M_PI/180;
}

double degree( double number )
{
	return number*180/M_PI;
}

void motor_usals(int fefd, double site_lat, double site_long, double sat_long,
	unsigned int verbose, int motor_wait_time)
{
	if (ioctl(fefd, FE_SET_TONE, SEC_TONE_OFF) == -1)
		printf("FE_SET_TONE ERROR! \n");
	usleep(20000);

	if (ioctl(fefd, FE_SET_VOLTAGE, SEC_VOLTAGE_18) == -1)
		printf("FE_SET_VOLTAGE ERROR! \n");
	usleep(20000);

	double r_eq = 6378.14;		// Earth radius
	double r_sat = 42164.57;	// Distance from earth centre to satellite

	site_lat  = radian(site_lat);
	site_long = radian(site_long);
	sat_long  = radian(sat_long);

	double declination = degree( atan( r_eq * sin(site_lat) / ( (r_sat - r_eq) + (r_eq * (1 - cos(site_lat))) ) ) );

	// x = [0], y = [1], z = [2]
	double dishVector[3] = { r_eq * cos(site_lat), 0, r_eq * sin(site_lat) };
	double satVector[3] = { r_sat * cos(site_long - sat_long), r_sat * sin(site_long - sat_long), 0 };
	double satPointing[3] = { satVector[0] - dishVector[0], satVector[1] - dishVector[1], satVector[2] - dishVector[2] } ;

	double motor_angle = degree( atan( satPointing[1]/satPointing[0] ) );

	int sixteenths = fabs(motor_angle) * 16.0 + 0.5;
	int angle_1, angle_2;
	angle_1 = motor_angle > 0.0 ? 0xd0 : 0xe0;
	angle_1 |= sixteenths >> 8;
	angle_2  = sixteenths & 0xff;

	printf("Long: %.2f, Lat: %.2f, Orbital Pos: %.2f, RotorCmd: %02x %02x, motor_angle: %.2f declination: %.2f\n", degree(site_long), degree(site_lat), degree(sat_long), angle_1, angle_2, motor_angle, declination);

	struct dvb_diseqc_master_cmd usals_cmd = { { 0xe0, 0x31, 0x6e, angle_1, angle_2, 0x00 }, 5 };

	sleep(1);
	diseqc_send_msg(fefd, usals_cmd, verbose);

//	printf("Waiting for motor to move, either wait 45sec or hit 's' to skip\n");
	printf("Waiting %d seconds for motor to move\n", motor_wait_time);
	sleep(motor_wait_time);

/*
    int c;
    int sec = time(NULL);
    set_conio_terminal_mode();
    do {
		sleep(1);
		if ( kbhit() )
	   		c = kbgetchar();
    } while( (char)c != 's' && sec+45 > time(NULL) );
	reset_terminal_mode();
*/
}

void motor_dir(int fefd, int dir, unsigned int verbose)
{
	diseqc_send_msg(fefd, dir_cmd[dir], verbose);
	usleep(20000);
}

void diseqc_send_msg(int fefd, struct dvb_diseqc_master_cmd cmd, unsigned int verbose)
{
	if (verbose) {
		printf("DiSEqC: %02x %02x %02x %02x %02x %02x length: %d\n",
			cmd.msg[0], cmd.msg[1],
			cmd.msg[2], cmd.msg[3],
			cmd.msg[4], cmd.msg[5], cmd.msg_len);
	}

	if (ioctl(fefd, FE_DISEQC_SEND_MASTER_CMD, &cmd) == -1)
		printf("FE_DISEQC_SEND_MASTER_CMD ERROR! \n");
	usleep(20000);
}

void setup_switch (int fefd, fe_sec_voltage_t voltage, fe_sec_tone_mode_t tone,
	int committed, int uncommitted, unsigned int verbose)
{
/*
	if (tone)
		printf("22khz OFF\n");
	else
		printf("22khz ON\n");
*/

	if (ioctl(fefd, FE_SET_TONE, SEC_TONE_OFF) == -1)
		printf("FE_SET_TONE ERROR! \n");
	usleep(20000);

	if (ioctl(fefd, FE_SET_VOLTAGE, voltage) == -1)
		printf("FE_SET_VOLTAGE ERROR! \n");
	usleep(20000);

	if (uncommitted)
		diseqc_send_msg(fefd, uncommitted_switch_cmds[uncommitted-1], verbose);

	if (committed)
		diseqc_send_msg(fefd, committed_switch_cmds[committed-1], verbose);

	if (ioctl(fefd, FE_SET_TONE, tone) == -1)
		printf("FE_SET_TONE ERROR! \n");
	usleep(20000);
}
