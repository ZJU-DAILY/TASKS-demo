bd:
	rm -rf build && mkdir build && cd build && cmake .. && make

flask:
	python ../test/flask_test.py &

run:
	cd build && ./multiKey

go:
	make bd && make run