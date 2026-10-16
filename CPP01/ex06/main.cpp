/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skayed <skayed@student.42roma.it>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 12:14:24 by skayed            #+#    #+#             */
/*   Updated: 2025/11/25 16:57:20 by skayed           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HarlFilter.hpp"

int main (int argc, char **argv){
	
	Harl h;
	std::string levels[4] = {
		"DEBUG",
		"INFO", 
		"WARNING", 
		"ERROR"
	};
	if (argc != 2)
		{
			std::cout << "Usage: write <DEBUG>, <INFO>, <WARNING> or <ERROR>" << std::endl ;
			return 1;
		}
	std::string command = argv[1];
	int index = 0;
	for (int i = 0; i < 4; i++){
		if (command == levels[i])
			break;
		index++;
	}
	switch (index){
		case 0:
			h.complain(0);
			// fall through
		case 1:
			h.complain(1);
			// fall through
		case 2:
			h.complain(2);
			// fall through
		case 3:
			h.complain(3);
			break;
		default:
			std::cout << "[ Probably complaining about insignificant problems ]" << std::endl;
			break;

	}
	return 0;
}