Summary of the Paper "The Anatomy of a Large-Scale Hypertextual Web Search Engine"

# Introduction and Why?

The paper discusses the need for an automatic indexer for the Web. It sheds light on how the rapid growing size of the web makes manually curated lists for the web not an option. Automated search engines relying on basic keyword matching includes too many false positives to be reliable. The papers solution to this problem is a Large Scale search engine named "Google". Google uses the structure provided by hypertext to solve the issues of unreliable results and uses them to give high quality results.

Unlike normal search engines of its time (late 1990s) Google is designed to scale with the web as it grows. It makes use of fast and efficient data structures to achieve this. Along with the assumption that the cost of indexing and storing HTML text is bound to decline by the advancements as hardware technology progresses.

# Goals

By the creation of Google, the team had 3 goals:

1. Improved Search Quality
2. Open Source instead of closed
   - Most search engine of the time had little to no specific publications of technical details
   - Google opposes to this and intends to publicize its technical specifications
3. Should be scalable such that reasonable number of people can use

# System Features

Among features such as location information, font size of words etc. Two features contribute greatly to the high quality results provided by Google.

## PageRank

PageRank uses the hypertext link (citation) to measure an objective importance of a citation that corresponds well with the objective quality for that citation.

Pagerank counts the number of citations and backlinks to a given page, normalized by the number of links on that page. Furthermore a damping factor is also introduced in order to simulate a random surfer which may get bored and go to a separate page. The damping factor also prevents loops on a part of the web as well. The PageRank for a given page can be calculated using a simple iterative algorithm.

A mental model for pagerank can be that a page having high value or PR(A) (Page rank for page A) that many pages point to that page or pages pointing to A have high value for PageRank themselves.

## Anchor Text

Anchor text refers to the linking of the text of the link to the page the link is on, along with the one it points to. This allows using these "anchors" to get a link for pages which cannot be indexed using text based methods, for example images, programs etc. This does create some bad results but these can be filtered so the end user rarely gets a bad result.

# System Anatomy

Google is implemented mostly in C and C++ due to the efficiency provided by these languages. One exception however is the usage of python for the Web Crawlers and URLServer.

# High level architecture overview

A URLServer sends a list of URLs to be fetched by the crawlers.

After fetching they are sent to the storeserver which compresses and stores them into a repository.

Every web page is assigned an ID called docID, assigned when a URL is parsed from a web page.

---

Indexing

Indexing is performed by the indexer and sorter.

The indexer reads pages from the repository, decompresses them and converts words into "hits". These hits store information such as position, font size etc.

The hits are stored in "barrels" to create "forward index".

The indexer also extracts links and anchor text from pages. The "URLResolver" converts these links into docIDs and creates a links database used to calculate PageRank.

The sorter reorganizes the forward index by wordID to create an "inverted index". The inverted index is used to quickly search documents containing a given word.

Finally, the searcher uses the inverted index, lexicon and PageRank to return ranked search results.

# Data Structures used in Google:

1. BigFiles
   - BigFiles are virtual files that can span across multiple file systems.
   - They use 64-bit addressing and automatically handle where data is stored.
   - BigFiles also support basic compression.
2. Repository
   - The repository stores the full HTML of every crawled web page.
   - Pages are compressed using zlib.
   - Each stored document contains: (docID, length, URL, compressed page content)
   - The repository is simple so that most other data structures can be rebuilt from it if required.
3. Document Index
   - The document index stores information about every document and is ordered by docID.
   - Each entry contains: document status, location in the repository, checksum, document statistics, URL and title etc.
   - Google also maintains a separate file for converting URLs into docIDs.
   - URLs are resolved in batches instead of disk lookup for each URL
4. Lexicon
   - The lexicon is a dictionary of known words.
   - It contains around 14 million words and is small enough to remain in memory.
   - It is made up of:
     - a list containing all words
     - a hash table containing pointers to those words
   - Keeping the lexicon in memory allows words to be looked up quickly.
5. Hit Lists
   - Each hit contains information like: word position, capitalization, font size, whether the word appears in an important location etc.
   - Hits are compressed into two bytes.
   - Hits can be of 2 types:
     1. Plain hits: normal occurrences in the document text.
     2. Fancy hits: occurrences in important locations such as the title, URL, anchor text etc.
6. Forward Index
   - The forward index maps documents to the words they contain.
   - It is essentially a map from "document to words"
   - It is divided into "barrels", where each barrel contains a range of wordIDs.
   - Each document stores its relevant wordIDs along with their hit lists.
   - The forward index is **partially sorted**, which makes creating the inverted index faster.
7. Inverted Index
   - The inverted index maps words to the documents that contain them.
   - It is essentially a map from "word to documents"
   - For each wordID, it stores a list of docIDs along with their corresponding hit lists.
   - Google keeps two sets of inverted barrels:
     1. One for important hits such as title and anchor text.
     2. One for all hits.
   - The searcher checks the important-hit barrels first and searches the larger set if more results are needed.

# Crawling the Web

Google uses a "distributed" crawling system. A URLServer sends URLs to multiple crawlers, with the URLServer and crawlers implemented in Python.

Each crawler keeps roughly 300 connections open at once. Each crawler also maintains its own DNS cache to reduce DNS lookup time.

The crawler uses asynchronous I/O and several queues to manage different stages such as DNS lookup, connecting to a server, sending requests and receiving responses.

A major difficulty with crawling is that web pages and servers can behave unexpectedly. Because of this, the crawler must be very robust and tested on a large portion of the web.

# Indexing the Web

After pages are crawled and stored, they must be parsed and indexed.

## Parsing

Google uses a fast lexical analyzer generated using flex, along with its own stack, instead of a more complex parser.

## Indexing Documents into Barrels

After parsing, every word in a document is converted into a wordID using the lexicon.

The occurrences of these words are then converted into hit lists and written into forward index barrels.

To allow multiple indexers to work in parallel, Google uses a fixed base lexicon and stores newly discovered words in separate log files which can later be processed.

## Sorting

The sorter converts the forward index into the inverted index.

Each forward barrel is sorted by wordID and produces:

- an inverted barrel containing title and anchor hits
- a full-text inverted barrel

Since the barrels may be too large to fit into memory, they are divided into smaller chunks called "baskets". Each basket is loaded into memory, sorted, and written back into the inverted index. These sorters can be run in parallel to improve efficiency

# Searching

Google focuses heavily on search quality while keeping the system scalable.

The search process works as follows:

1. Parse the query.
2. Convert query words into wordIDs.
3. Find the corresponding document lists in the short inverted barrels.
4. Scan the lists to find documents containing all query terms.
5. Calculate the rank of each matching document.
6. If there are not enough results, search the full inverted barrels.
7. Sort the matching documents by rank.
8. Return the top results.

To limit response time, Google stops after finding a certain number of matching documents, even though this may sometimes produce slightly worse results.

## Ranking System

Google ranks documents using several types of information, including:

- PageRank
- position of words
- font size
- capitalization
- title and URL hits
- anchor text
- frequency of words

For a single-word query, each occurrence is given a type depending on where it appears, such as title, anchor text, URL or normal text.

Each type has a different weight. The number of occurrences is also considered, although repeated occurrences eventually give less additional benefit.

These values are combined into an IR score, which is then combined with PageRank to calculate the final document rank.

For multi-word queries, Google also considers the "proximity" of the query words. Closer proximity words are given more importance than far away words.

**## Feedback**

The ranking system contains many adjustable weights and parameters, far too much to get right in a single time.

Google uses feedback from trusted users to evaluate search results.

This feedback is saved so that when the ranking function is changed, Google can compare the new results with previously evaluated searches.

This helps determine whether changes to the ranking system actually improve search quality.

# Results and Performance

Google's results showed that using PageRank, anchor text and word proximity could produce more relevant search results than many search engines of the time.

The paper gives example for the query "bill clinton", Google returned several pages from the White House website. Some results were found mainly through anchor text even when the page itself had not been crawled.

The results also showed the importance of proximity. Pages containing "Bill" and "Clinton" close together were ranked higher than pages where the words appeared separately.

## Storage Requirements

Google was designed to use storage efficiently.

The fetched pages originally required about 148 GB, but compression reduced the repository to about 53.5 GB.

The rest of the search engine data, including indexes, document information and links, required about 55 GB.

The short inverted index was only about 4.1 GB and could answer most queries without using the much larger full inverted index.

## System Performance

Google's main operations were crawling, indexing and sorting.

The system can crawl around 48.5 pages per second.

The indexer processes roughly 54 pages per second, which was slightly faster than the crawlers and prevented indexing from becoming a bottleneck.

Sorting was performed in parallel. Using four machines, the entire sorting process took about 24 hours.

## Search Performance

At the time of the paper, most queries took between 1 and 10 seconds.

Much of this delay came from disk I/O across multiple machines.

Google had not implemented several common optimizations.

The paper assumes that with better hardware, algorithmic improvements Google could handle several hundred queries per second.

# Conclusion

The paper presents Google as both a practical search engine and a research platform for studying large-scale web search.

Its future work mainly focuses on improving efficiency, keeping the index updated, and adding features such as query caching, smarter recrawling, clustering, user context and result summarization.

The paper also plans to improve the use of link information, including personalized PageRank and using surrounding link text.

Overall, the paper argues that search quality and scalability must be considered together when designing a large-scale search engine. Google was built to support both while also providing a platform for further research into web search and information retrieval.
