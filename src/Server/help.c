/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Server help messages
*/

#include <stdio.h>

/**
 * @brief Display the help message for the server
 */
void display_help(void)
{
    printf("USAGE: ./zappy_server -p port -x width -y height -n name1 "
        "name2 ... -c clientsNb -f freq --auto-start on|off --display-eggs "
        "true|false\n");
    printf("[-v | --verbose]--game_duration time\n");
    printf("\tport\t\tis the port number\n");
    printf("\twidth\t\tis the width of the world\n");
    printf("\theight\t\tis the height of the world\n");
    printf("\tnameX\t\tis the name of the team X\n");
    printf("\tclientsNb\tis the number of authorized clients per team\n");
    printf("\tfreq\t\tis the reciprocal of time unit for execution of "
        "actions\n");
    printf("\tauto-start\tdoes the greeting is send automaticly\n");
    printf("\tdisplay-eggs\teggs are visible and destructible\n");
    printf("\tmatch_duration\tis the duration of the match in seconds\n");
}
