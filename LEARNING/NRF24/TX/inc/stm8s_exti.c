ignment */
  {
    /* Read MSB first*/
    temph = ADC2->DRH;
    /* Then read LSB */
    templ = ADC2->DRL;
    
    temph = (uint16_t)((uint16_t)((uint16_t)templ << 6) | (uint16_t)((uint16_t)temph << 8));
  }
  
  return ((uint16_t)temph);
}

/**
  * @brief  Checks the ADC2 EOC flag status.
  * @param  None
  * @retval FlagStatus Status of the ADC2 EOC flag.
  */
FlagStatus ADC2_GetFlagStatus(void)
{
  /* Get EOC  flag status */
  return (FlagStatus)(ADC2->CSR & ADC2_CSR_EOC);
}

/**
  * @brief  Clear the ADC2 EOC Flag.
  * @param  None
  * @retval None
  */
void ADC2_ClearFlag(void)
{
  ADC2->CSR &= (uint8_t)(~ADC2_CSR_EOC);
}

/**
  * @brief  Returns the EOC  pending bit status
 * @par Parameters:
  * None
  * @retval FlagStatus: status of the EOC pending bit.
  */
ITStatus ADC2_GetITStatus(void)
{
  return (ITStatus)(ADC2->CSR & ADC2_CSR_EOC);
}

/**
  * @brief  Clear the ADC2 End of Conversion pending bit.
  * @param  None
  * @retval None
  */
void ADC2_ClearITPendingBit(void)
{
  ADC2->CSR &= (uint8_t)(~ADC2_CSR_EOC);
}

/**
  * @}
  */
  
/**
  * @}
  */
  
#endif /* (STM8S208) ||(STM8S207) || (STM8S007) || (STM8AF52Ax) || (STM8AF62Ax) */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                PK  ;4�H               epl-v10.html�;ks�8���못�[�l'N&�ĩ��t�:Y�Jr2��Q��	������~��)93���a&&	4��B���.ƃ�盀_ͮ�����a8�ݣ�O/GG���w|t�޳w�"���$N�Yv~P�陎V2���(�t�,�Q��e˥���瀙���c�����`U볣��f�ۼ�e���lr4	]D����X b�,��]�G���YZȴ�ζky�#�t~Pȯ�nˣ�ȵ,�����ͫ_�'Ha��/<�����ew�t�V�\��(Vk-���qp3���8>~������åJE��%l�G��v���Y�"���v~S�c�!���c���VY�Oz���Z�N[��o�.�G�@��K��k��#��%��.E$�O��~Z�4Ӳ{r��������������?}�~Jt֥��c��Z���^�J�c���.E�˳W�^�����_^�ӟO߼|}���������ӟ����SD����w��Q�'"��X�Ou_?-x41��9���� ��JώUZ=t�YQ��􎏏Oօ�z-�����6j�m�Y�^	�e��oyv�wL���x����U�N H�u���".�DYd�/���G�!�K�cQ��*�8���+q����^48��D�د?�p޽�x"����Oe�[N �����<�GP���{�g�-�ܲ��WXfY���Z����|jr�C�;s����:r�ud�{7�[���� uo����?+ļ��u��"�s��r8�G���܁Ʒk.b��s0k�PA t�Ù������þ��x�9b�&���i0l�Ӗc�³��������M�9}�7��I���S��cx\���E0a�tL��||�gW� Ûi�|?������Qf����I\��y<�q8��N�� ��v0�#>���p:���ޚg�T�ƣ�,��΂)�ބ ���nf�� `���H	�~�k6���'=~\����Z �K�Wyi��Б�y��5ox"�6NV	*�� �.�(͟�+�B��<�ْ�&�	P+|����e�E���,*�.�[�m��/S�(ؤ4���R��y7�})�sD�=��
RD+�˹��
�|4�*��`F�C�)���as�gw�H�Ou*+���}�C7+�K�K`�#H<�r�*<����Y�e�%$y�ܓ7�oa�(8��BEe,r_n`���n����Y��z��\|#4"��BN#�-�^�xɑ�t�����"�;��^�L�\�x�k`�A�y���Q\�����������?W��-�ldO�-JH�P�1�ܴ�����eJޛmT�j��F��g���ϢaBp�=����^2Hb�hg�f��鑾(�}W����4����J��S7��u��|��F`���(�c`*іI�<�^4��ʰJFRk�2�-�a��j tR[�K-��"&��TW�B<�"L�*��m�}'����jC7YK�DA�"�H:4|W��\��/**
  ******************************************************************************
  * @file    stm8s_awu.c
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   This file contains all the functions for the AWU peripheral.  
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm8s_awu.h"

/** @addtogroup STM8S_StdPeriph_Driver
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions -----------------------------------------