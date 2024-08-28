import random
branch_bsz = 8
B.<L0_0, L0_1, L0_2, L0_3, L0_4, L0_5, L0_6, L0_7, L1_0, L1_1, L1_2, L1_3, L1_4, L1_5, L1_6, L1_7, R0_0, R0_1, R0_2, R0_3, R0_4, R0_5, R0_6, R0_7, R1_0, R1_1, R1_2, R1_3, R1_4, R1_5, R1_6, R1_7> = BooleanPolynomialRing(32)

rot_amt_a = 1
rot_amt_b = 7
rot_amt_c = 2

ROUND     = 32

#Delare Branches
L0 = [L0_0, L0_1, L0_2, L0_3, L0_4, L0_5, L0_6, L0_7]
L1 = [L1_0, L1_1, L1_2, L1_3, L1_4, L1_5, L1_6, L1_7]
R0 = [R0_0, R0_1, R0_2, R0_3, R0_4, R0_5, R0_6, R0_7]
R1 = [R1_0, R1_1, R1_2, R1_3, R1_4, R1_5, R1_6, R1_7]

def F(L, R):
    #Left Rotate 1
    ROLa_L = L[rot_amt_a:] + L[:rot_amt_a]
    #Left Rotate 7
    ROLb_L = L[rot_amt_b:] + L[:rot_amt_b]
    #Left Rotate 2
    ROLc_L = L[rot_amt_c:] + L[:rot_amt_c]
    
    #And
    RST_AND = []
    for idx in range(len(L)):
        RST_AND.append(ROLa_L[idx] * ROLb_L[idx])
    #XOR
    RST_R = []
    for idx in range(len(L)):
        RST_R.append(R[idx] + RST_AND[idx] + ROLc_L[idx]+random.randint(0, 1))
    
    return list(RST_R)


def round_func(L0, L1, R0, R1):
    L1 = F(L0, L1)
    L0 = F(L1, L0)
    
    R1 = F(R0, R1)
    R0 = F(R1, R0)
    return list(R1), list(L0), list(L1), list(R0)




for ridx in range(ROUND):
    L0, L1, R0, R1 = round_func(L0, L1, R0, R1)
    print(ridx + 1, "Round")
    deg_L0 = []
    deg_L1 = []
    deg_R0 = []
    deg_R1 = []
    for bidx in range(branch_bsz):
        deg_L0.append(L0[bidx].degree())
        deg_L1.append(L1[bidx].degree())
        deg_R0.append(R0[bidx].degree())
        deg_R1.append(R1[bidx].degree())
    print(deg_L0)
    print(deg_L1)
    print(deg_R0)
    print(deg_L1)
    max_deg = max(deg_L0 + deg_L1 + deg_R0 + deg_R1)
    min_deg = min(deg_L0 + deg_L1 + deg_R0 + deg_R1)
    print(max_deg, min_deg)
    print("============")
