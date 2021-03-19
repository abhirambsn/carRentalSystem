crSystem: date.o auth.o car.o customer.o rentals.o main.o
	g++ date.o auth.o car.o customer.o rentals.o main.o -l sqlite3 -l ncurses -Wall -Wextra -g -o crSystem

date.o:	Date/date.cpp
	g++ -Wall -Wextra -g -c Date/date.cpp

car.o:	Car/car.cpp
	g++ -Wall -Wextra -g -c Car/car.cpp

customer.o:	Customer/customer.cpp
	g++ -Wall -Wextra -g -c Customer/customer.cpp

auth.o: Auth/auth.cpp
	g++ -Wall -Wextra -g -c Auth/auth.cpp


rentals.o: Rental/rentals.cpp
	g++ -Wall -Wextra -g -c Rental/rentals.cpp

main.o: main.cpp
	g++ -Wall -Wextra -g -c main.cpp

clean:
	rm -r *.o crSystem

install:
	mkdir -p /usr/local/bin/crSystem
	cp *.o crSystem /usr/local/bin/crSystem
	ln -s /usr/local/bin/crSystem/crSystem /usr/local/bin/crSystem

uninstall:
	rm -r /usr/local/bin/crSystem
	rm /usr/local/bin/crSystem