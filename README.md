# shadow verification
Source code for Redefining Security in Shadow Cipher for __IoT Nodes: New Full-round Practical Distinguisher and the Infeasibility of Key Recovery Attacks__ 

Shadow_32_degree.py and shadow_degree_bound.py need to be run in a SageMath environment.

When Shadow_32_degree.py is executed, it outputs the algebraic degree of the Shadow_32 cipher for each round based on a randomly selected key.

When shadow_degree_bound.py is executed, it outputs the upper bounds of the algebraic degree for each branch of Shadow_32 and Shadow_64.

Building and running the shadow.c and shadow.h files in the shadow_verification folder performs experimental verification for Lemma 1 and Corollary 5.1.

The test_lemma1 function first randomly selects a key and then uses TEST_NUM plaintexts to calculate the value given in Lemma 1, verifying whether it matches for all plaintexts.

The test_corollary5_1 function checks whether the integral bit given in Corollary 5.1 becomes zero by randomly selecting TEST_NUM keys.
