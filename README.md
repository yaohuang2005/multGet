## multiGet
multiGet is a file downloader by chunk and assembly

## Design Consideration
The architecture of multiGet is multiple producers - single consumer application.
Unlike classic producer-consumer structure, multiGet does not use a queue to link
the producers and consumer, but in the consumer, multiGet has a stream buffer to
keep the received bytes in order.

The reasons are:
1. The consumer function is only to dump the message to file, it
   does not need to process (parse) the message.

2. For performance, if we use queue, even we can save the message byte stream
   ptr into queue, but if the consumer thread is not selected to execute or slow down,
   the message that is pointed by ptr might be lose. If we save the message byte stream instance
   in the queue, it is a time consuming task to cope bytes.

3. Library dependency on Mac or Linux:
       libcurl (for http "get")
       glibc++ (for getopt)
       thread (for multithread)

Assumption:
Since this is a simple application,
1. logging information is send to stdout, no ogging to file or syslog-ng ...
2. configuration is read from command line, no conf property file is need


So the diagram is:

 MultiGet(main) -> FileChunkDownloader(mutilthreaded) -> writer(singleton with stream buffer) 


## build step: (on MacOS)

```
Pre-requirement libraries installation:
(on my MacOS)
brew install libcurl

In terminal:

$git clone https://github.com/yaohuang2005/multiGet.git
$cd multiGet


The directory structure is:
build/ src/ CMakeList.txt README.md


$cd build
$cmake ..
$make
$cd src

## test
$./multiGet -h
$./multiGet -u http://5d9a03a0.bwtest-aws.pravala.com/384MB.jar
$./multiGet -u http://5d9a03a0.bwtest-aws.pravala.com/384MB.jar -o receivedFile
$./multiGet -u http://5d9a03a0.bwtest-aws.pravala.com/384MB.jar -o receivedFile -s 1024


Integration test 1:

multiGet -u http://5d9a03a0.bwtest-aws.pravala.com/384MB.jar -o test.txt
URL set to: http://5d9a03a0.bwtest-aws.pravala.com/384MB.jar
Output file set to: test.txt
only pull 4194304 byte, even the file has 402653352 byte
To get url file size is 4194304
create thread to pull file chunk
The #2 thread is writing 3833 byte to buffer
The #2 thread is writing 7240 byte to buffer
The #3 thread is writing 4344 byte to buffer
The #3 thread is writing 1448 byte to buffer
The #2 thread is writing 5792 byte to buffer
The #1 thread is writing 1448 byte to buffer
.
.
.
The #3 thread is writing 4344 byte to buffer
The #3 thread is writing 2896 byte to buffer
The #3 thread is writing 2896 byte to buffer
The #3 thread is writing 5792 byte to buffer
The #3 thread is writing 5415 byte to buffer

Final report:
only pull 4194304 byte, even the file has 402653352 byte
Received url file byte 4194304

Process finished with exit code 0


Integration test 2:

WTL-EN-EEG8WL:src yhuang$ ./multiGet -u http://www.tehhayley.com/blog/2012/partial-http-downloads-with-curl/
URL set to: http://www.tehhayley.com/blog/2012/partial-http-downloads-with-curl/
the file has only 5689 byte, so we can only pull this size
To get url file size is 5689
create thread to pull file chunk
The #0 thread is writing 1422 byte to buffer
The #3 thread is writing 1423 byte to buffer
The #1 thread is writing 1422 byte to buffer
The #2 thread is writing 1422 byte to buffer

Final report:
the file has only 5689 byte, so we can only pull this size
Received url file byte 5689


You can verify the file content with the content by:
curl -O http://www.tehhayley.com/blog/2012/partial-http-downloads-with-curl/

```
