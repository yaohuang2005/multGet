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


## build step:

```
$ cd cmamke-build-debug
$ cmake ..
$ make

## test
$./multiGet -h
$./multiGet -u http://5d9a03a0.bwtest-aws.pravala.com/384MB.jar
$./multiGet -u http://5d9a03a0.bwtest-aws.pravala.com/384MB.jar -o receivedFile
$./multiGet -u http://5d9a03a0.bwtest-aws.pravala.com/384MB.jar -o receivedFile -s 1024

```
