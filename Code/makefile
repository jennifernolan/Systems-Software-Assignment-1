CC=gcc
objects = daemon.o lockfiles.o backup.o update.o timestamp.o TrackChanges.o
headers = lockfiles.h backup.h update.h timestamp.h TrackChanges.h

daemon : $(objects)
	$(CC) -o daemon $(objects) -lm -lrt

daemon.o : daemon.c $(headers)
	$(CC) -c daemon.c

lockfiles.o : lockfiles.c
	$(CC) -c lockfiles.c

backup.o : backup.c timestamp.h
	$(CC) -c backup.c

update.o : update.c
	$(CC) -c update.c

timestamp.o : timestamp.c
	$(CC) -c timestamp.c

TrackChanges.o : TrackChanges.c
	$(CC) -c TrackChanges.c

clean:
	rm daemon $(objects)
