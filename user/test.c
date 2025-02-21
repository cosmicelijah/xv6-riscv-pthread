#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "user/threads.h"
#include "user/test.h"

// Threading function for Test 1
void *test_1_thread(void *arg) {
  int tid = *(int *)arg;

  printf("Hello from thread %d!\n", tid);

  // Returns a null pointer, NOT the integer 0
  return 0;
}

void test_1(void) {
  printf("\n----------= Starting Test 1 =----------\n");
  printf("Spawn 1 thread with TID 0 and joins it\n");
  printf("---------------------------------------\n");

  thread_t thread;
  int tid = 0;
  void *ignored = 0;

  if (thread_create(&thread, test_1_thread, &tid) == -1) {
  	printf("Test 1: failed to create thread!\n");
  	exit(1);
  }

  if (thread_join(&thread, &ignored) == -1) {
  	printf("Test 1: did not join thread\n");
  	exit(1);
  }
  
  printf("----------=  Passed Test 1  =----------\n");
}

// Threading function for Test 2
void *test_2_thread(void *arg) {
  int tid = *(int *)arg;

  printf("Hello from thread %d!\n", tid);

  return 0;
}

void test_2(void) {
  printf("\n----------= Starting Test 2 =----------\n");
  printf("Spawn 8 threads with TID 0-7 and joins them\n");
  printf("---------------------------------------\n");
  
  thread_t threads[8];
  int tids[8] = {0, 1, 2, 3, 4, 5, 6, 7};

  for (int i = 0; i < 8; i++) {
    if (thread_create(&threads[i], test_2_thread, &tids[i]) == -1) {
      printf("Test 2: failed to create thread %d!\n", tids[i]);
      exit(1);
    }
  }

  for (int i = 0; i < 8; i++) {
    void *ignored = 0;
    if (thread_join(&threads[i], &ignored) == -1) {
      printf("Test 2: did not join thread %d!\n", tids[i]);
      exit(1);
    }
  }

  printf("----------=  Passed Test 2  =----------\n");
}

// Threading function for Test 3
void *test_3_thread(void *arg) {
  int tid = *(int *)arg;

  printf("Hello from thread %d!\n", tid);

  int *retval = malloc(sizeof(int));

  *retval = tid;

  return retval;
}

void test_3(void) {
  printf("\n----------= Starting Test 3 =----------\n");
  printf("Spawn 8 threads with TID 0-7 and each thread returns its TID\n");
  printf("---------------------------------------\n");

  int NUM_THREADS = 8;
  
  thread_t threads[NUM_THREADS];
  int tids[] = {0, 1, 2, 3, 4, 5, 6, 7};
  int *retvals[NUM_THREADS];

  for (int i = 0; i < NUM_THREADS; i++) {
    if (thread_create(&threads[i], &test_3_thread, &tids[i]) == -1) {
      printf("Test 3: failed to create thread %d!\n", tids[i]);
      exit(1);
    }
  }
 
  for (int i = 0; i < NUM_THREADS; i++) {
    if (thread_join(&threads[i], (void **)(&retvals[i])) == -1) {
      printf("Test 3: did not join thread %d!\n", tids[i]);
    }
    printf("Thread %d returned %d!\n", tids[i], *retvals[i]);
    assert(*retvals[i] == i);
  }

  printf("----------=  Passed Test 3  =----------\n");
}

char *page1 = 0;
char *page2 = 0;

// Threading function for Test 4
void *test_4_thread0(void *arg) {
  int tid = *(int *)arg;

  printf("Hello from thread %d!\n", tid);

  printf("Thread %d: Allocating new pages...\n", tid);

  // Use sbrk to force a new page to be made
  page1 = sbrk(4096); // One page of memory

  page2 = sbrk(4096); // Another page of memory

  char values1[] = {'D', 'E', 'A', 'D'};
  char values2[] = {'B', 'E', 'E', 'F'};

  printf("Thread %d: Putting values in page 1...\n", tid);

  for (int i = 0; i < 4; i++) {
  	page1[i] = values1[i];
  }

  printf("Thread %d: Putting values in page 2...\n", tid);

  for (int i = 0; i < 4; i++) {
  	page2[i] = values2[i];
  }

  printf("Thread %d: Values successfully placed. Now exiting...\n", tid);

  return 0;
}

// Threading function for Test 4
void *test_4_thread1(void *arg) {
  int tid = *(int *)arg;

  printf("Hello from thread %d!\n", tid);

  printf("Thread %d: going to sleep!\n", tid);

  sleep(10);

  printf("Thread %d: waking up and starting to read the page!\n", tid);

  char values1[5] = {0};
  char values2[5] = {0};

  for (int i = 0; i < 4; i++) {
  	values1[i] = page1[i];
  }

  printf("Thread %d: successfully read page 1! The first 4 values are %s!\n", tid, values1);

  for (int i = 0; i < 4; i++) {
  	values2[i] = page2[i];
  }

  printf("Thread %d: successfully read page 2! The first 4 values are %s!\n", tid, values2);

  printf("Thread %d: Freeing both pages!\n", tid);

  // Free both pages at once
  sbrk(-4096 * 2);
  
  printf("Thread %d: exiting!\n", tid);

  return 0;
}

void test_4(void) {
  printf("\n----------= Starting Test 4 =----------\n");
  printf("Spawns 2 threads with TIDs 0 and 1.\n");
  printf("Thread 0 creates a page and adds data to it.\n");
  printf("Thread 1 sleeps for 10 and then attempts to read the data.\n");
  printf("Neither thread returns anything.\n");
  printf("---------------------------------------\n");
  
  thread_t threads[2];
  int tids[2] = {0, 1};
  int *ignored;

  // Create thread 0
  if (thread_create(&threads[0], &test_4_thread0, &tids[0]) == -1) {
    printf("Test 4: failed to create thread %d!\n", tids[0]);
    exit(1);
  }

  // Create thread 1
  if (thread_create(&threads[1], &test_4_thread1, &tids[1]) == -1) {
    printf("Test 4: failed to create thread %d!\n", tids[1]);
    exit(1);
  }
 
  for (int i = 0; i < 2; i++) {
    if (thread_join(&threads[i], (void **)(&ignored)) == -1) {
      printf("Test 4: did not join thread %d!\n", tids[i]);
    }
    printf("Joining thread %d\n", tids[i]);
  }

  printf("----------=  Passed Test 4  =----------\n");
}

volatile int sum = 0;

// Threading function for Test 5
void *test_5_thread(void *arg) {
  int amt_to_add = *(int *)arg;
  
  for (int i = 0; i < amt_to_add; i++) {
  	sum++;
  }
  
  return 0;
}

void test_5(void) {
  int num_threads = 16;

  printf("\n----------= Starting Test 5 =----------\n");
  printf("Spawn %d threads to parallel add to a shared sum\n", num_threads);
  printf("---------------------------------------\n");

  thread_t thread[num_threads];
  int amount_to_add = 100000000;
  void *ignored = 0;

  for (int i = 0; i < num_threads; i++) {
	if (thread_create(&thread[i], test_5_thread, &amount_to_add) == -1) {
	  printf("Test 5: failed to create thread!\n");
	  exit(1);
	}
  }

  for (int i = 0; i < num_threads; i++) {
	if (thread_join(&thread[i], &ignored) == -1) {
	  printf("Test 5: failed to join thread!\n");
	  exit(1);
	}
  }

  printf("\nFinal sum (should be %d): %d\n", num_threads * amount_to_add, sum);
  
  printf("----------=  Passed Test 5  =----------\n");
}

// Threading function for Test 6
void *test_6_thread(void *arg) {
  int tid = *(int *)arg;

  printf("Hello from thread %d! Exiting now!\n", tid);

  // Returns a null pointer, NOT the integer 0
  return 0;
}

// Threading function for Test 6
void *test_6_thread1(void *arg) {
  int tid = *(int *)arg;

  printf("Hello from thread %d! Spinning forever now!\n", tid);

  // Spin forever
  for(;;);
 

  // Returns a null pointer, NOT the integer 0
  return 0;
}


void test_6(void) {
  printf("\n----------= Starting Test 6 =----------\n");
  printf("Spawn 1 thread with TID 0 which spins forever\n");
  printf("Parent calls cancel after sleeping for 50\n");
  printf("---------------------------------------\n");

  thread_t thread;
  thread_t thread1;
  int tid = 0;
  int tid1 = 1;

  if (thread_create(&thread, test_6_thread, &tid) == -1) {
  	printf("Test 6: failed to create thread!\n");
  	exit(1);
  }

  if (thread_create(&thread1, test_6_thread1, &tid1) == -1) {
  	printf("Test 6: failed to create thread1!\n");
  	exit(1);
  }

  sleep(10);

  if (thread_cancel(&thread)) {
  	printf("Test 6: did not cancel thread\n");
  	exit(1);
  } else {
  	printf("Cancelled thread %d\n", tid);
  }

  if (thread_cancel(&thread1)) {
  	printf("Test 6: did not cancel thread\n");
  	exit(1);
  } else {
  	printf("Cancelled thread %d\n", tid1);
  }
  
  printf("----------=  Passed Test 6  =----------\n");
}

static int NUM_TESTS = 6;

// 0 means all
static int test_to_run = 0;

int main(int argc, char **argv) {
  if (argc > 1) {
  	int test = atoi(argv[1]);
  	if (test > 0 && test <= NUM_TESTS) {
  	  test_to_run = test;
  	}
  }

  void (*tests[])(void) = { test_1, test_2, test_3, test_4, test_5, test_6 };

  if (test_to_run == 0) {
  	for (int i = 0; i < NUM_TESTS; i++) {
  	  // if (i == 4) {
  	  //   // Skip test 5 in series because it takes forever
  	  //   printf("\n---------=  Skipped Test 5  =----------\n");
  	  // 	continue;
  	  // }
  	  tests[i]();
  	}
  } else {
  	tests[test_to_run - 1]();
  }

  return 0;
}
