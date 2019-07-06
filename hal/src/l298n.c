/*
 * l298n.c
 *
 *  Created on: Jul 6, 2019
 *      Author: truongtx
 */
#include "l298n.h"
#include "gpio.h"

// private function prototype
void turnLeft();
void turnRight();
void up();
void down();
void stop();

L298Config *m_config;

void L298_init(L298Config* config)
{
    m_config = config;
    pin_mode(m_config->INA, OUTPUT);
    pin_mode(m_config->INB, OUTPUT);
    pin_mode(m_config->INC, OUTPUT);
    pin_mode(m_config->IND, OUTPUT);
}

void L298_drive(Navigation nav)
{
    switch(nav)
    {
        case Left:
        {
            turnLeft();
            break;
        }
        case Right:
        {
            turnRight();
            break;
        }
        case Up:
        {
            up();
            break;
        }
        case Down:
        {
            down();
            break;
        }
        default:
        {
            stop();
            break;
        }

    }
}

void turnLeft()
{
    digital_write(m_config->INA, LOW);
    digital_write(m_config->INB, LOW);
    digital_write(m_config->INC, HIGH);
    digital_write(m_config->IND, LOW);
}

void turnRight()
{
    digital_write(m_config->INA, HIGH);
    digital_write(m_config->INB, LOW);
    digital_write(m_config->INC, LOW);
    digital_write(m_config->IND, LOW);
}

void up()
{
    digital_write(m_config->INA, HIGH);
    digital_write(m_config->INB, LOW);
    digital_write(m_config->INC, HIGH);
    digital_write(m_config->IND, LOW);
}

void down()
{
    digital_write(m_config->INA, LOW);
    digital_write(m_config->INB, HIGH);
    digital_write(m_config->INC, LOW);
    digital_write(m_config->IND, HIGH);
}

void stop()
{
    digital_write(m_config->INA, LOW);
    digital_write(m_config->INB, LOW);
    digital_write(m_config->INC, LOW);
    digital_write(m_config->IND, LOW);
}
