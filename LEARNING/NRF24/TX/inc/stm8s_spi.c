                           
#define AFIO_MAPR_SWJ_CFG_DISABLE_Msk        (0x1U << AFIO_MAPR_SWJ_CFG_DISABLE_Pos) /*!< 0x04000000 */
#define AFIO_MAPR_SWJ_CFG_DISABLE            AFIO_MAPR_SWJ_CFG_DISABLE_Msk     /*!< JTAG-DP Disabled and SW-DP Disabled */


/*****************  Bit definition for AFIO_EXTICR1 register  *****************/
#define AFIO_EXTICR1_EXTI0_Pos               (0U)                              
#define AFIO_EXTICR1_EXTI0_Msk               (0xFU << AFIO_EXTICR1_EXTI0_Pos)  /*!< 0x0000000F */
#define AFIO_EXTICR1_EXTI0                   AFIO_EXTICR1_EXTI0_Msk            /*!< EXTI 0 configuration */
#define AFIO_EXTICR1_EXTI1_Pos               (4U)                              
#define AFIO_EXTICR1_EXTI1_Msk               (0xFU << AFIO_EXTICR1_EXTI1_Pos)  /*!< 0x000000F0 */
#define AFIO_EXTICR1_EXTI1                   AFIO_EXTICR1_EXTI1_Msk            /*!< EXTI 1 configuration */
#define AFIO_EXTICR1_EXTI2_Pos               (8U)                              
#define AFIO_EXTICR1_EXTI2_Msk               (0xFU << AFIO_EXTICR1_EXTI2_Pos)  /*!< 0x00000F00 */
#define AFIO_EXTICR1_EXTI2                   AFIO_EXTICR1_EXTI2_Msk            /*!< EXTI 2 configuration */
#define AFIO_EXTICR1_EXTI3_Pos               (12U)                             
#define AFIO_EXTICR1_EXTI3_Msk               (0xFU << AFIO_EXTICR1_EXTI3_Pos)  /*!< 0x0000F000 */
#define AFIO_EXTICR1_EXTI3                   AFIO_EXTICR1_EXTI3_Msk            /*!< EXTI 3 configuration */

/*!< EXTI0 configuration */
#define AFIO_EXTICR1_EXTI0_PA                0x00000000U                          /*!< PA[0] pin */
#define AFIO_EXTICR1_EXTI0_PB_Pos            (0U)                              
#define AFIO_EXTICR1_EXTI0_PB_Msk            (0x1U << AFIO_EXTICR1_EXTI0_PB_Pos) /*!< 0x00000001 */
#define AFIO_EXTICR1_EXTI0_PB                AFIO_EXTICR1_EXTI0_PB_Msk         /*!< PB[0] pin */
#define AFIO_EXTICR1_EXTI0_PC_Pos            (1U)                              
#define AFIO_EXTICR1_EXTI0_PC_Msk            (0x1U << AFIO_EXTICR1_EXTI0_PC_Pos) /*!< 0x00000002 */
#define AFIO_EXTICR1_EXTI0_PC                AFIO_EXTICR1_EXTI0_PC_Msk         /*!< PC[0] pin */
#define AFIO_EXTICR1_EXTI0_PD_Pos            (0U)                              
#define AFIO_EXTICR1_EXTI0_PD_Msk            (0x3U << AFIO_EXTICR1_EXTI0_PD_Pos) /*!< 0x00000003 */
#define AFIO_EXTICR1_EXTI0_PD                AFIO_EXTICR1_EXTI0_PD_Msk         /*!< PD[0] pin */
#define AFIO_EXTICR1_EXTI0_PE_Pos            (2U)                              
#define AFIO_EXTICR1_EXTI0_PE_Msk            (0x1U << AFIO_EXTICR1_EXTI0_PE_Pos) /*!< 0x00000004 */
#define AFIO_EXTICR1_EXTI0_PE                AFIO_EXTICR1_EXTI0_PE_Msk         /*!< PE[0] pin */
#define AFIO_EXTICR1_EXTI0_PF_Pos            (0U)                              
#define AFIO_EXTICR1_EXTI0_PF_Msk            (0x5U << AFIO_EXTICR1_EXTI0_PF_Pos) /*!< 0x00000005 */
#define AFIO_EXTICR1_EXTI0_PF                AFIO_EXTICR1_EXTI0_PF_Msk         /*!< PF[0] pin */
#define AFIO_EXTICR1_EXTI0_PG_Pos            (1U)                              
#define AFIO_EXTICR1_EXTI0_PG_Msk            (0x3U << AFIO_EXTICR1_EXTI0_PG_Pos) /*!< 0x00000006 */
#define AFIO_EXTICR1_EXTI0_PG                AFIO_EXTICR1_EXTI0_PG_Msk         /*!< PG[0] pin */

/*!< EXTI1 configuration */
#define AFIO_EXTICR1_EXTI1_PA                0x00000000U                          /*!< PA[1] pin */
#define AFIO_EXTICR1_EXTI1_PB_Pos            (4U)                              
#define AFIO_EXTICR1_EXTI1_PB_Msk            (0x1U << AFIO_EXTICR1_EXTI1_PB_Pos) /*!< 0x00000010 */
#define AFIO_EXTICR1_EXTI1_PB                AFIO_EXTICR1_EXTI1_PB_Msk         /*!< PB[1] pin */
#define AFIO_EXTICR1_EXTI1_PC_Pos            (5U)                              
#define AFIO_EXTICR1_EXTI1_PC_Msk            (0x1U << AFIO_EXTICR1_EXTI1_PC_Pos) /*!< 0x00000020 */
#define AFIO_EXTICR1_EXTI1_PC                AFIO_EXTICR1_EXTI1_PC_Msk         /*!< PC[1] pin */
#define AFIO_EXTICR1_EXTI1_PD_Pos            (4U)                              
#define AFIO_EXTICR1_EXTI1_PD_Msk            (0x3U << AFIO_EXTICR1_EXTI1_PD_Pos) /*!< 0x00000030 */
#define AFIO_EXTICR1_EXTI1_PD                AFIO_EXTICR1_EXTI1_PD_Msk         /*!< PD[1] pin */
#define AFIO_EXTICR1_EXTI1_PE_Pos            (6U)                              
#define AFIO_EXTICR1_EXTI1_PE_Msk            (0x1U << AFIO_EXTICR1_EXTI1_PE_Pos) /*!< 0x00000040 */
#define AFIO_EXTICR1_EXTI1_PE                AFIO_EXTICR1_EXTI1_PE_Msk         /*!< PE[1] pin */
#define AFIO_EXTICR1_EXTI1_PF_Pos            (4U)                              
#define AFIO_EXTICR1_EXTI1_PF_Msk            (0x5U << AFIO_EXTICR1_EXTI1_PF_Pos) /*!< 0x00000050 */
#define AFIO_EXTICR1_EXTI1_PF                AFIO_EXTICR1_EXTI1_PF_Msk         /*!< PF[1] pin */
#define AFIO_EXTICR1_EXTI1_PG_Pos            (5U)                              
#define AFIO_EXTICR1_EXTI1_PG_Msk            (0x3U << AFIO_EXTICR1_EXTI1_PG_Pos) /*!< 0x00000060 */
#define AFIO_EXTICR1_EXTI1_PG                AFIO_EXTICR1_EXTI1_PG_Msk         /*!< PG[1] pin */

/*!< EXTI2 configuration */  
#define AFIO_EXTICR1_EXTI2_PA                0x00000000U                          /*!< PA[2] pin */
#define AFIO_EXTICR1_EXTI2_PB_Pos            (8U)                              
#define AFIO_EXTICR1_EXTI2_PB_Msk            (0x1U << AFIO_EXTICR1_EXTI2_PB_Pos) /*!< 0x00000100 */
#define AFIO_EXTICR1_EXTI2_PB                AFIO_EXTICR1_EXTI2_PB_Msk         /*!< PB[2] pin */
#define AFIO_EXTICR1_EXTI2_PC_Pos            (9U)                              
#define AFIO_EXTICR1_EXTI2_PC_Msk            (0x1U << AFIO_EXTICR1_EXTI2_PC_Pos) /*!< 0x00000200 */
#define AFIO_EXTICR1_EXTI2_PC                AFIO_EXTICR1_EXTI2_PC_Msk         /*!< PC[2] pin */
#define AFIO_EXTICR1_EXTI2_PD_Pos            (8U)                              
#define AFIO_EXTICR1_EXTI2_PD_Msk            (0x3U << AFIO_EXTICR1_EXTI2_PD_Pos) /*!< 0x00000300 */
#define AFIO_EXTICR1_EXTI2_PD                AFIO_EXTICR1_EXTI2_PD_Msk         /*!< PD[2] pin */
#define AFIO_EXTICR1_EXTI2_PE_Pos            (10U)                             
#define AFIO_EXTICR1_EXTI2_PE_Msk            (0x1U << AFIO_EXTICR1_EXTI2_PE_Pos) /*!< 0x00000400 */
#define AFIO_EXTICR1_EXTI2_PE                AFIO_EXTICR1_EXTI2_PE_Msk         /*!< PE[2] pin */
#define AFIO_EXTICR1_EXTI2_PF_Pos            (8U)                              
#define AFIO_EXTICR1_EXTI2_PF_Msk            (0x5U << AFIO_EXTICR1_EXTI2_PF_Pos) /*!< 0x00000500 */
#define AFIO_EXTICR1_EXTI2_PF                AFIO_EXTICR1_EXTI2_PF_Msk         /*!< PF[2] pin */
#define AFIO_EXTICR1_EXTI2_PG_Pos            (9U)                              
#define AFIO_EXTICR1_EXTI2_PG_Msk            (0x3U << AFIO_EXTICR1_EXTI2_PG_Pos) /*!< 0x00000600 */
#define AFIO_EXTICR1_EXTI2_PG                AFIO_EXTICR1_EXTI2_PG_Msk         /*!< PG[2] pin */

/*!< EXTI3 configuration */
#define AFIO_EXTICR1_EXTI3_PA                0x00000000U                          /*!< PA[3] pin */
#define AFIO_EXTICR1_EXTI3_PB_Pos            (12U)                             
#define AFIO_EXTICR1_EXTI3_PB_Msk            (0x1U << AFIO_EXTICR1_EXTI3_PB_Pos) /*!< 0x00001000 */
#define AFIO_EXTICR1_EXTI3_PB                AFIO_EXTICR1_EXTI3_PB_Msk         /*!< PB[3] pin */
#define AFIO_EXTICR1_EXTI3_PC_Pos            (13U)                             
#define AFIO_EXTICR1_EXTI3_PC_Msk            (0x1U << AFIO_EXTICR1_EXTI3_PC_Pos) /*!< 0x00002000 */
#define AFIO_EXTICR1_EXTI3_PC                AFIO_EXTICR1_EXTI3_PC_Msk         /*!< PC[3] pin */
#define AFIO_EXTICR1_EXTI3_PD_Pos            (12U)                             
#define AFIO_EXTICR1_EXTI3_PD_Msk            (0x3U << AFIO_EXTICR1_EXTI3_PD_Pos) /*!< 0x00003000 */
#define AFIO_EXTICR1_EXTI3_PD                AFIO_EXTICR1_EXTI3_PD_Msk         /*!< PD[3] pin */
#define AFIO_EXTICR1_EXTI3_PE_Pos            (14U)                             
#define AFIO_EXTICR1_EXTI3_PE_Msk            (0x1U << AFIO_EXTICR1_EXTI3_PE_Pos) /*!< 0x00004000 */
#define AFIO_EXTICR1_EXTI3_PE                AFIO_EXTICR1_EXTI3_PE_Msk         /*!< PE[3] pin */
#define AFIO_EXTICR1_EXTI3_PF_Pos            (12U)                             
#define AFIO_EXTICR1_EXTI3_PF_Msk            (0x5U << AFIO_EXTICR1_EXTI3_PF_Pos) /*!< 0x00005000 */
#define AFIO_EXTICR1_EXTI3_PF                AFIO_EXTICR1_EXTI3_PF_Msk         /*!< PF[3] pin */
#define AFIO_EXTICR1_EXTI3_PG_Pos            (13U)                             
#define AFIO_EXTICR1_EXTI3_PG_Msk            (0x3U << AFIO_EXTICR1_EXTI3_PG_Pos) /*!< 0x00006000 */
#define AFIO_EXTICR1_EXTI3_PG                AFIO_EXTICR1_EXTI3_PG_Msk         /*!< PG[3] pin */

/*****************  Bit definition for AFIO_EXTICR2 register  *****************/
#define AFIO_EXTICR2_EXTI4_Pos               (0U)                              
#define AFIO_EXTICR2_EXTI4_Msk               (0xFU << AFIO_EXTICR2_EXTI4_Pos)  /*!< 0x0000000F */
#define AFIO_EXTICR2_EXTI4                   AFIO_EXTICR2_EXTI4_Msk            /*!< EXTI 4 configuration */
#define AFIO_EXTICR2_EXTI5_Pos               (4U)                              
#define AFIO_EXTICR2_EXTI5_Msk               (0xFU << AFIO_EXTICR2_EXTI5_Pos)  /*!< 0x000000F0 */
#define AFIO_EXTICR2_EXTI5                   AFIO_EXTICR2_EXTI5_Msk            /*!< EXTI 5 configuration */
#define AFIO_EXTICR2_EXTI6_Pos               (8U)                              
#define AFIO_EXTICR2_EXTI6_Msk               (0xFU << AFIO_EXTICR2_EXTI6_Pos)  /*!< 0x00000F00 */
#define AFIO_EXTICR2_EXTI6                   AFIO_EXTICR2_EXTI6_Msk            /*!< EXTI 6 configuration */
#define AFIO_EXTICR2_EXTI7_Pos               (12U)                             
#define AFIO_EXTICR2_EXTI7_Msk               (0xFU << AFIO_EXTICR2_EXTI7_Pos)  /*!< 0x0000F000 */
#define AFIO_EXTICR2_EXTI7                   AFIO_EXTICR2_EXTI7_Msk            /*!< EXTI 7 configuration */

/*!< EXTI4 configuration */
#define AFIO_EXTICR2_EXTI4_PA                0x00000000U                          /*!< PA[4] pin */
#define AFIO_EXTICR2_EXTI4_PB_Pos            (0U)                              
#define AFIO_EXTICR2_EXTI4_PB_Msk            (0x1U << AFIO_EXTICR2_EXTI4_PB_Pos) /*!< 0x00000001 */
#define AFIO_EXTICR2_EXTI4_PB                AFIO_EXTICR2_EXTI4_PB_Msk         /*!< PB[4] pin */
#define AFIO_EXTICR2_EXTI4_PC_Pos            (1U)                              
#define AFIO_EXTICR2_EXTI4_PC_Msk            (0x1U << AFIO_EXTICR2_EXTI4_PC_Pos) /*!< 0x00000002 */
#define AFIO_EXTICR2_EXTI4_PC                AFIO_EXTICR2_EXTI4_PC_Msk         /*!< PC[4] pin */
#define AFIO_EXTICR2_EXTI4_PD_Pos            (0U)                              
#define AFIO_EXTICR2_EXTI4_PD_Msk            (0x3U << AFIO_EXTICR2_EXTI4_PD_Pos) /*!< 0x00000003 */
#define AFIO_EXTICR2_EXTI4_PD                AFIO_EXTICR2_EXTI4_PD_Msk         /*!< PD[4] pin */
#define AFIO_EXTICR2_EXTI4_PE_Pos            (2U)                              
#define AFIO_EXTICR2_EXTI4_PE_Msk            (0x1U << AFIO_EXTICR2_EXTI4_PE_Pos) /*!< 0x00000004 */
#define AFIO_EXTICR2_EXTI4_PE                AFIO_EXTICR2_EXTI4_PE_Msk         /*!< PE[4] pin */
#define AFIO_EXTICR2_EXTI4_PF_Pos            (0U)                              
#define AFIO_EXTICR2_EXTI4_PF_Msk            (0x5U << AFIO_EXTICR2_EXTI4_PF_Pos) /*!< 0x00000005 */
#define AFIO_EXTICR2_EXTI4_PF                AFIO_EXTICR2_EXTI4_PF_Msk         /*!< PF[4] pin */
#define AFIO_EXTICR2_EXTI4_PG_Pos            (1U)                              
#define AFIO_EXTICR2_EXTI4_PG_Msk            (0x3U << AFIO_EXTICR2_EXTI4_PG_Pos) /*!< 0x00000006 */
#define AFIO_EXTICR2_EXTI4_PG                AFIO_EXTICR2_EXTI4_PG_Msk         /*!< PG[4] pin */

/* EXTI5 configuration */
#define AFIO_EXTICR2_EXTI5_PA                0x00000000U                          /*!< PA[5] pin */
#define AFIO_EXTICR2_EXTI5_PB_Pos            (4U)                              
#define AFIO_EXTICR2_EXTI5_PB_Msk            (0x1U << AFIO_EXTICR2_EXTI5_PB_Pos) /*!< 0x00000010 */
#define AFIO_EXTICR2_EXTI5_PB                AFIO_EXTICR2_EXTI5_PB_Msk         /*!< PB[5] pin */
#define AFIO_EXTICR2_EXTI5_PC_Pos            (5U)                              
#define AFIO_EXTICR2_EXTI5_PC_Msk            (0x1U << AFIO_EXTICR2_EXTI5_PC_Pos) /*!< 0x00000020 */
#define AFIO_EXTICR2_EXTI5_PC                AFIO_EXTICR2_EXTI5_PC_Msk         /*!< PC[5] pin */
#define AFIO_EXTICR2_EXTI5_PD_Pos            (4U)                              
#define AFIO_EXTICR2_EXTI5_PD_Msk            (0x3U << AFIO_EXTICR2_EXTI5_PD_Pos) /*!< 0x00000030 */
#define AFIO_EXTICR2_EXTI5_PD                AFIO_EXTICR2_EXTI5_PD_Msk         /*!< PD[5] pin */
#define AFIO_EXTICR2_EXTI5_PE_Pos            (6U)                              
#define AFIO_EXTICR2_EXTI5_PE_Msk            (0x1U << AFIO_EXTICR2_EXTI5_PE_Pos) /*!< 0x00000040 */
#define AFIO_EXTICR2_EXTI5_PE                AFIO_EXTICR2_EXTI5_PE_Msk         /*!< PE[5] pin */
#define AFIO_EXTICR2_EXTI5_PF_Pos            (4U)                              
#define AFIO_EXTICR2_EXTI5_PF_Msk            (0x5U << AFIO_EXTICR2_EXTI5_PF_Pos) /*!< 0x00000050 */
#define AFIO_EXTICR2_EXTI5_PF                AFIO_EXTICR2_EXTI5_PF_Msk         /*!< PF[5] pin */
#define AFIO_EXTICR2_EXTI5_PG_Pos            (5U)                              
#define AFIO_EXTICR2_EXTI5_PG_Msk            (0x3U << AFIO_EXTICR2_EXTI5_PG_Pos) /*!< 0x00000060 */
#define AFIO_EXTICR2_EXTI5_PG                AFIO_EXTICR2_EXTI5_PG_Msk         /*!< PG[5] pin */

/*!< EXTI6 configuration */  
#define AFIO_EXTICR2_EXTI6_PA                0x00000000U                          /*!< PA[6] pin */
#define AFIO_EXTICR2_EXTI6_PB_Pos            (8U)                              
#define AFIO_EXTICR2_EXTI6_PB_Msk            (0x1U << AFIO_EXTICR2_EXTI6_PB_Pos) /*!< 0x00000100 */
#define AFIO_EXTICR2_EXTI6_PB            