#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "user/threads.h"
#include "user/test.h"



void *start_fn(void *arg) {
  int tid = *(int *)arg;

  printf("Thread %d: Hello from start!\n", tid);
  
  // sleep(10)
  
  int *ret = malloc(sizeof(int));

  // switch (tid[0]) {
  // 	case 'a':
  // 	  *ret = 10;
  // 	  break;
  // 	case 'b':
  // 	  *ret = 11;
  // 	  break;
  // 	case 'c':
  // 	  *ret = 12;
  // 	  break;
  // 	case 'd':
  // 	  *ret = -1;
  // 	  break;
  // 	default:
  // 	  *ret = 255;
  // }

  *ret = 0;

  printf("Thread %d exiting!\n", tid);

  return ret;
}

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

char *page = 0;

// Threading function for Test 4
void *test_4_thread0(void *arg) {
  int tid = *(int *)arg;

  printf("Hello from thread %d!\n", tid);

  printf("Thread %d: Allocating new page...\n", tid);

  // Alloc huge amount of memory to force new pages to be made
  page = malloc(65536);

  //ignored[0] = 4;
  
  //page = malloc(4096);

  // printf("Thread %d: Address of new page is %p\n", tid, ignored);
  printf("Thread %d: Address of new page is %p\n", tid, page);

  char values[] = {'D', 'E', 'A', 'D', 'B', 'E', 'E', 'F'};
  int len = 8;

  printf("Thread %d: Putting values in new page...\n", tid);

  for (int i = 0; i < len; i++) {
  	page[i] = values[i];
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

  char values[8];

  for (int i = 0; i < 8; i++) {
  	values[i] = page[i];
  }
  // values[len + 1] = 0; // Null terminate for printing

  printf("Thread %d: successfully read the page! The first 8 values are %s!\n", tid, values);

  printf("Thread %d: Freeing page and exiting!\n", tid);

  free(page);

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
  }

  printf("----------=  Passed Test 4  =----------\n");
}

// static int NUM_THREADS = 1;
static int NUM_TESTS = 4;

// 0 means all
static int test_to_run = 0;

int main(int argc, char **argv) {
  if (argc > 1) {
  	int test = atoi(argv[1]);
  	if (test > 0 && test <= NUM_TESTS) {
  	  test_to_run = test;
  	}
  }


  void (*tests[])(void) = { test_1, test_2, test_3, test_4 };

  if (test_to_run == 0) {
  	for (int i = 0; i < NUM_TESTS; i++) {
  	  tests[i]();
  	}
  } else {
  	tests[test_to_run - 1]();
  }


  //printf("Pagesize: %d\n", PGSIZE);

  // --------= Initialize necessary thread data =--------


  
  return 0;
}
