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
       pthread (for multithread)


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


Test result:
./multiGet -u http://5d9a03a0.bwtest-aws.pravala.com/384MB.jar -o test.txt
.
.
.
only pull 4194304 byte, even the file has 402653352 byte
create thread to pull file chunk
The #0 thread is writing 5276 byte to buffer
The #0 thread is writing 4344 byte to buffer
The #0 thread is writing 5792 byte to buffer
The #0 thread is writing 1448 byte to buffer
The #0 thread is writing 4344 byte to buffer
The #0 thread is writing 1448 byte to buffer
The #0 thread is writing 1448 byte to buffer
The #0 thread is writing 4344 byte to buffer
The #0 thread is writing 2896 byte to buffer
The #0 thread is writing 4344 byte to buffer
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


```
