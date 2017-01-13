lrunt.so: lrunt.c
	gcc -fPIC -shared $< -o $@ -llua -lrunt

clean: 
	rm -rf lrunt.so
