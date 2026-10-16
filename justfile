build:
    cc src/*.c util/*.c -o target/main -Wall -Werror -std=gnu11 -lncurses

init:
    just build && rm -rf .fvm && ./target/main init

add:
    ./target/main add LICENSE

commit:
    ./target/main commit -m "Testing Commit"

commit2:
    ./target/main commit -m "Next Commit"

checkout:
    ./target/main checkout HEAD~1

test:
    just init && just add && just commit && just commit2 && just checkout

check-commit:
    just build && valgrind --leak-check=yes ./target/main commit
