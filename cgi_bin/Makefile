FIND_PATH=-I/home/zy/3_5/sql_connect -I/home/zy/3_5/sql_connect/mysql_lib/include -L/home/zy/3_5/sql_connect/mysql_lib/lib -lmysqlclient

.PHONY:all
all:debug_cgi show_student_cgi register_cgi


debug_cgi:debug_cgi.c
	gcc -o $@ $^
show_student_cgi:student_show_cgi.cpp /home/zy/3_5/sql_connect/sql_connect.cpp 
	g++ -o $@ $^ ${FIND_PATH}
register_cgi:register_cgi.cpp /home/zy/3_5/sql_connect/sql_connect.cpp 
	g++ -o $@ $^ ${FIND_PATH}

.PHONY:output
output:
	cp debug_cgi ../output/htdocs/cgi_bin
	cp show_student_cgi ../output/htdocs/cgi_bin
	cp register_cgi ../output/htdocs/cgi_bin

.PHONY:clean
clean:
	rm -rf debug_cgi show_student_cgi register_cgi
