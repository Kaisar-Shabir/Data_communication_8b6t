# Data_communication_8b6t
In 8B6T we encode a pattern of 8 bits as a pattern of 6 signals, where the signal has three signal levels rather than 2 in the
original data.
Since the original data unit is of length 8 bits and has 2 signal levels, we can have 2 8 different data patterns where as each
signal unit has length of 6 and 3 levels, we can have 3 6 signal patterns.
There are 729 − 256 = 473 redundant signal elements that provide synchronization and error detection. Redundancy is also
used to provide DC balance.
Each signal pattern has a weight of 0 or +1 DC values. This means that there is no pattern with the weight −1. To make the
whole stream DC-balanced, the sender keeps track of the weight. If two groups of weight 1 are encountered one after
another, the first one is sent as is, while the next one is totally inverted to give a weight of −1.
