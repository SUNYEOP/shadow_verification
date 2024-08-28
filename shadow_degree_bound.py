import random
B.<L0_0, L0_1, L0_2, L0_3, L0_4, L0_5, L0_6, L0_7, L1_0, L1_1, L1_2, L1_3, L1_4, L1_5, L1_6, L1_7, R0_0, R0_1, R0_2, R0_3, R0_4, R0_5, R0_6, R0_7, R1_0, R1_1, R1_2, R1_3, R1_4, R1_5, R1_6, R1_7> = BooleanPolynomialRing(32)

shadow_32_deg=(L0_0+R0_0)*(L0_2+R0_2)*(L0_4+R0_4)*(L0_6+R0_6)
shadow_32_deg=shadow_32_deg*(L0_2*L0_0+R0_2*R0_0+L0_3+R0_3+L1_1+R1_1)
shadow_32_deg=shadow_32_deg*(L0_4*L0_2+R0_4*R0_2+L0_5+R0_5+L1_3+R1_3)
shadow_32_deg=shadow_32_deg*(L0_6*L0_4+R0_6*R0_4+L0_7+R0_7+L1_5+R1_5)
shadow_32_deg=shadow_32_deg*(L0_0*L0_6+R0_0*R0_6+L0_1+R0_1+L1_7+R1_7)
print("degree upperbound of shadow_32 :", "L0,R0 :", 2 + shadow_32_deg.degree(),"L1,R1 :", 4 + shadow_32_deg.degree())


B.<L0_0, L0_1, L0_2, L0_3, L0_4, L0_5, L0_6, L0_7,L0_8, L0_9, L0_10, L0_11, L0_12, L0_13, L0_14, L0_15,L1_0, L1_1, L1_2, L1_3, L1_4, L1_5, L1_6, L1_7,L1_8, L1_9, L1_10, L1_11, L1_12, L1_13, L1_14, L1_15,R0_0, R0_1, R0_2, R0_3, R0_4, R0_5, R0_6, R0_7,R0_8, R0_9, R0_10, R0_11, R0_12, R0_13, R0_14, R0_15,R1_0, R1_1, R1_2, R1_3, R1_4, R1_5, R1_6, R1_7,R1_8, R1_9, R1_10, R1_11, R1_12, R1_13, R1_14, R1_15>= BooleanPolynomialRing(64)

shadow_64_deg=(L0_0+R0_0)*(L0_2+R0_2)*(L0_4+R0_4)*(L0_6+R0_6)*(L0_8+R0_8)*(L0_10+R0_10)*(L0_12+R0_12)*(L0_14+R0_14)
shadow_64_deg=shadow_64_deg*(L0_2*L0_8+R0_2*R0_8+L0_3+R0_3+L1_1+R1_1)
shadow_64_deg=shadow_64_deg*(L0_4*L0_10+R0_4*R0_10+L0_5+R0_5+L1_3+R1_3)
shadow_64_deg=shadow_64_deg*(L0_6*L0_12+R0_6*R0_12+L0_7+R0_7+L1_5+R1_5)
shadow_64_deg=shadow_64_deg*(L0_8*L0_14+R0_8*R0_14+L0_9+R0_9+L1_7+R1_7)
shadow_64_deg=shadow_64_deg*(L0_10*L0_0+R0_10*R0_0+L0_11+R0_11+L1_9+R1_9)
shadow_64_deg=shadow_64_deg*(L0_12*L0_2+R0_12*R0_2+L0_13+R0_13+L1_11+R1_11)
shadow_64_deg=shadow_64_deg*(L0_14*L0_4+R0_14*R0_4+L0_15+R0_15+L1_13+R1_13)
shadow_64_deg=shadow_64_deg*(L0_0*L0_6+R0_0*R0_6+L0_1+R0_1+L1_15+R1_15)
print("degree upperbound of shadow_64 :", "L0,R0 :", 2 + shadow_64_deg.degree(),"L1,R1 :", 4 + shadow_64_deg.degree())
