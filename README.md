# PageRank Search Engine
This project implements a search engine. The search engine takes webpage data as input. Then it allows the user to enter single-word searches. The search engine then returns an importance-ordered list of webpages containing that word.

## Getting started: compile and run
To compile this project using the following command:

```
g++ PageRankSearchEngine.cpp -std=c++11 -Wall -o searchEngine
```

To run use the following command:

```
> [executable] [webpage_data_file]
```

The webpage data input is given as a command-line argument. The argument is the location to the file with the webpage data. A sample data set is given in `./data/sample/webpages.txt`.

Once the intial processing is done, you will be prompted to enter a search word. After you do, an importance-ordered list of webpages containing that word are returned.

## Sample webpage data
A sample data set is given in `./data/sample/webpages.txt`. This data is scraped from [Clemson University's](https://clemson.edu) webpages. It conains data from 69664 webpages including words and links on each page.

## Webpage data file format
If you would like to use your own web data set, you can! Here is the format you should follow for your data file:

```
NEWPAGE [page_1_url]
[word_or_link_on_page_1]
...
[word_or_link_on_page_1]
NEWPAGE [page_2_url]
...
```

The list of words and links for each page can be any word or URL. Links are detected by containing `http://`. If it doesn't contain that, it is considered a word.

The links on pages don't need to be internal (i.e. they can link to a page not in the list of `NEWPAGE` pages). Any external links are just ignored.

## How it works
There are four steps to the program:
1. Read pages - read input webpage data file and store in memory
2. Rank pages - use PageRank algorithm to rank every page by importance
3. Index pages - preprocess word index
4. Get input - continuously prompt for search word input

### 1. Read pages
Because external links need to be ignored, a first pass of the file gets the list of URLs in the network. Each `NEWPAGE` URL is considered a node in the network.

After getting the list of URLs in the network, a second pass gets the list of links and words on each page. The links are considered arcs in the network.

### 2. Rank pages
The program uses the ubiquitous [PageRank](https://wikipedia.org/wiki/PageRank) algorithm to rank the nodes in the network. The damping factor *d = 0.9*.

### 3. Index pages
The program preprocess a word index. The index is a hash table which maps words to a list of pages which contain that word. The domain of the hash table is the union of all words which appear on any page in the webpage data file.

### 4. Get input
The program prompts the user for a search word input. The search engine then returns an importance-ordered list of webpages containing that word. Then input is requested again. Use your command-line termination shortcut to terminate.