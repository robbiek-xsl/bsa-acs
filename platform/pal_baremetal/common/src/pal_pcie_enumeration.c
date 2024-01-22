/** @file
 * Copyright (c) 2023, Arm Limited or its affiliates. All rights reserved.
 * SPDX-License-Identifier : Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
**/

#include "pal_pcie_enum.h"
#include "pal_common_support.h"
#include "platform_override_struct.h"

extern PCIE_INFO_TABLE *g_pcie_info_table;

uint32_t pcie_index = 0, enumerate = 1;
/*64-bit address initialisation*/
uint64_t g_bar64_p_start  = PLATFORM_OVERRIDE_PCIE_BAR64_VAL;
uint64_t g_rp_bar64_value = PLATFORM_OVERRIDE_RP_BAR64_VAL;
uint64_t g_bar64_p_max;
uint32_t g_64_bus, g_bar64_size;

/*32-bit address initialisation*/
uint32_t g_bar32_np_start = PLATFORM_OVERRIDE_PCIE_BAR32NP_VAL;
uint32_t g_bar32_p_start  = PLATFORM_OVERRIDE_PCIE_BAR32P_VAL;
uint32_t g_rp_bar32_value = PLATOFRM_OVERRIDE_RP_BAR32_VAL;
uint32_t g_bar32_np_max;
uint32_t g_bar32_p_max;
uint32_t g_np_bar_size = 0, g_p_bar_size = 0;
uint32_t g_np_bus = 0, g_p_bus = 0;

PCIE_ROOT_TABLE pcie_root_table = {
    .num_entries = ROOT_PORT_COUNT,
#if ROOT_PORT_COUNT > 0
    .block[0].segment =       PLATFORM_OVERRIDE_ROOT_SEGMENT_VAL_0,
    .block[0].start_bus_num = PLATFORM_OVERRIDE_ROOT_START_BUS_VAL_0,
    .block[0].end_bus_num =   PLATFORM_OVERRIDE_ROOT_END_BUS_VAL_0,
    .block[0].bar64_val =     PLATFORM_OVERRIDE_ROOT_PCIE_BAR64_VAL_0,
    .block[0].rp_bar64_val =  PLATFORM_OVERRIDE_ROOT_RP_BAR64_VAL_0,
    .block[0].bar32np_val =   PLATFORM_OVERRIDE_ROOT_PCIE_BAR32NP_VAL_0,
    .block[0].bar32p_val =    PLATFORM_OVERRIDE_ROOT_PCIE_BAR32P_VAL_0,
    .block[0].rp_bar32_val =  PLATFORM_OVERRIDE_ROOT_RP_BAR32_VAL_0,
#endif
#if ROOT_PORT_COUNT > 1
    .block[1].segment =       PLATFORM_OVERRIDE_ROOT_SEGMENT_VAL_1,
    .block[1].start_bus_num = PLATFORM_OVERRIDE_ROOT_START_BUS_VAL_1,
    .block[1].end_bus_num =   PLATFORM_OVERRIDE_ROOT_END_BUS_VAL_1,
    .block[1].bar64_val =     PLATFORM_OVERRIDE_ROOT_PCIE_BAR64_VAL_1,
    .block[1].rp_bar64_val =  PLATFORM_OVERRIDE_ROOT_RP_BAR64_VAL_1,
    .block[1].bar32np_val =   PLATFORM_OVERRIDE_ROOT_PCIE_BAR32NP_VAL_1,
    .block[1].bar32p_val =    PLATFORM_OVERRIDE_ROOT_PCIE_BAR32P_VAL_1,
    .block[1].rp_bar32_val =  PLATFORM_OVERRIDE_ROOT_RP_BAR32_VAL_1,
#endif
#if ROOT_PORT_COUNT > 2
    .block[2].segment =       PLATFORM_OVERRIDE_ROOT_SEGMENT_VAL_2,
    .block[2].start_bus_num = PLATFORM_OVERRIDE_ROOT_START_BUS_VAL_2,
    .block[2].end_bus_num =   PLATFORM_OVERRIDE_ROOT_END_BUS_VAL_2,
    .block[2].bar64_val =     PLATFORM_OVERRIDE_ROOT_PCIE_BAR64_VAL_2,
    .block[2].rp_bar64_val =  PLATFORM_OVERRIDE_ROOT_RP_BAR64_VAL_2,
    .block[2].bar32np_val =   PLATFORM_OVERRIDE_ROOT_PCIE_BAR32NP_VAL_2,
    .block[2].bar32p_val =    PLATFORM_OVERRIDE_ROOT_PCIE_BAR32P_VAL_2,
    .block[2].rp_bar32_val =  PLATFORM_OVERRIDE_ROOT_RP_BAR32_VAL_2,
#endif
#if ROOT_PORT_COUNT > 3
    .block[3].segment =       PLATFORM_OVERRIDE_ROOT_SEGMENT_VAL_3,
    .block[3].start_bus_num = PLATFORM_OVERRIDE_ROOT_START_BUS_VAL_3,
    .block[3].end_bus_num =   PLATFORM_OVERRIDE_ROOT_END_BUS_VAL_3,
    .block[3].bar64_val =     PLATFORM_OVERRIDE_ROOT_PCIE_BAR64_VAL_3,
    .block[3].rp_bar64_val =  PLATFORM_OVERRIDE_ROOT_RP_BAR64_VAL_3,
    .block[3].bar32np_val =   PLATFORM_OVERRIDE_ROOT_PCIE_BAR32NP_VAL_3,
    .block[3].bar32p_val =    PLATFORM_OVERRIDE_ROOT_PCIE_BAR32P_VAL_3,
    .block[3].rp_bar32_val =  PLATFORM_OVERRIDE_ROOT_RP_BAR32_VAL_3,
#endif
#if ROOT_PORT_COUNT > 4
    .block[4].segment =       PLATFORM_OVERRIDE_ROOT_SEGMENT_VAL_4,
    .block[4].start_bus_num = PLATFORM_OVERRIDE_ROOT_START_BUS_VAL_4,
    .block[4].end_bus_num =   PLATFORM_OVERRIDE_ROOT_END_BUS_VAL_4,
    .block[4].bar64_val =     PLATFORM_OVERRIDE_ROOT_PCIE_BAR64_VAL_4,
    .block[4].rp_bar64_val =  PLATFORM_OVERRIDE_ROOT_RP_BAR64_VAL_4,
    .block[4].bar32np_val =   PLATFORM_OVERRIDE_ROOT_PCIE_BAR32NP_VAL_4,
    .block[4].bar32p_val =    PLATFORM_OVERRIDE_ROOT_PCIE_BAR32P_VAL_4,
    .block[4].rp_bar32_val =  PLATFORM_OVERRIDE_ROOT_RP_BAR32_VAL_4,
#endif
#if ROOT_PORT_COUNT > 5
    .block[5].segment =       PLATFORM_OVERRIDE_ROOT_SEGMENT_VAL_5,
    .block[5].start_bus_num = PLATFORM_OVERRIDE_ROOT_START_BUS_VAL_5,
    .block[5].end_bus_num =   PLATFORM_OVERRIDE_ROOT_END_BUS_VAL_5,
    .block[5].bar64_val =     PLATFORM_OVERRIDE_ROOT_PCIE_BAR64_VAL_5,
    .block[5].rp_bar64_val =  PLATFORM_OVERRIDE_ROOT_RP_BAR64_VAL_5,
    .block[5].bar32np_val =   PLATFORM_OVERRIDE_ROOT_PCIE_BAR32NP_VAL_5,
    .block[5].bar32p_val =    PLATFORM_OVERRIDE_ROOT_PCIE_BAR32P_VAL_5,
    .block[5].rp_bar32_val =  PLATFORM_OVERRIDE_ROOT_RP_BAR32_VAL_5,
#endif
#if ROOT_PORT_COUNT > 6
    .block[6].segment =       PLATFORM_OVERRIDE_ROOT_SEGMENT_VAL_6,
    .block[6].start_bus_num = PLATFORM_OVERRIDE_ROOT_START_BUS_VAL_6,
    .block[6].end_bus_num =   PLATFORM_OVERRIDE_ROOT_END_BUS_VAL_6,
    .block[6].bar64_val =     PLATFORM_OVERRIDE_ROOT_PCIE_BAR64_VAL_6,
    .block[6].rp_bar64_val =  PLATFORM_OVERRIDE_ROOT_RP_BAR64_VAL_6,
    .block[6].bar32np_val =   PLATFORM_OVERRIDE_ROOT_PCIE_BAR32NP_VAL_6,
    .block[6].bar32p_val =    PLATFORM_OVERRIDE_ROOT_PCIE_BAR32P_VAL_6,
    .block[6].rp_bar32_val =  PLATFORM_OVERRIDE_ROOT_RP_BAR32_VAL_6,
#endif
#if ROOT_PORT_COUNT > 7
    .block[7].segment =       PLATFORM_OVERRIDE_ROOT_SEGMENT_VAL_7,
    .block[7].start_bus_num = PLATFORM_OVERRIDE_ROOT_START_BUS_VAL_7,
    .block[7].end_bus_num =   PLATFORM_OVERRIDE_ROOT_END_BUS_VAL_7,
    .block[7].bar64_val =     PLATFORM_OVERRIDE_ROOT_PCIE_BAR64_VAL_7,
    .block[7].rp_bar64_val =  PLATFORM_OVERRIDE_ROOT_RP_BAR64_VAL_7,
    .block[7].bar32np_val =   PLATFORM_OVERRIDE_ROOT_PCIE_BAR32NP_VAL_7,
    .block[7].bar32p_val =    PLATFORM_OVERRIDE_ROOT_PCIE_BAR32P_VAL_7,
    .block[7].rp_bar32_val =  PLATFORM_OVERRIDE_ROOT_RP_BAR32_VAL_7,
#endif
};

PCIE_ROOT_TABLE *g_pcie_root_table = &pcie_root_table;

/**
  @brief   This API reads 32-bit data from PCIe config space pointed by Bus,
           Device, Function and register offset.
  @param   bus,dev,func -  Bus(8-bits), device(8-bits) & function(8-bits)
  @param   offset - Register offset within a device PCIe config space
  @param   *data  - 32-bit data read from the config space
  @return  success/failure
**/
uint32_t pal_pci_cfg_read(uint32_t bus, uint32_t dev, uint32_t func, uint32_t offset, uint32_t *value)
{

  uint32_t cfg_addr;
  uint32_t i = 0;
  if (!enumerate)
  {
      while (i < g_pcie_info_table->num_entries)
      {
         if ((bus >= g_pcie_info_table->block[i].start_bus_num) &&
              (bus <= g_pcie_info_table->block[i].end_bus_num))
         {
             pcie_index = i;
             break;
         }
         i++;
      }
  }

  uint64_t ecam_base = g_pcie_info_table->block[pcie_index].ecam_base;

  cfg_addr = (bus * PCIE_MAX_DEV * PCIE_MAX_FUNC * PCIE_CFG_SIZE) + (dev * PCIE_MAX_FUNC * PCIE_CFG_SIZE) + (func * PCIE_CFG_SIZE);
  *value = pal_mmio_read(ecam_base + cfg_addr + offset);
  return 0;

}

/**
  @brief   This API writes 32-bit data to PCIe config space pointed by Bus,
           Device, Function and register offset.
  @param   bus,dev,func - Bus(8-bits), device(8-bits) & function(8-bits)
  @param   offset - Register offset within a device PCIe config space
  @param   data   - data to be written to the config space
  @return  None
**/
void pal_pci_cfg_write(uint32_t bus, uint32_t dev, uint32_t func, uint32_t offset, uint32_t data)
{

  uint64_t ecam_base = g_pcie_info_table->block[pcie_index].ecam_base;

  uint32_t cfg_addr;

  cfg_addr = (bus * PCIE_MAX_DEV * PCIE_MAX_FUNC * PCIE_CFG_SIZE) + (dev * PCIE_MAX_FUNC * PCIE_CFG_SIZE) + (func * PCIE_CFG_SIZE);

  pal_mmio_write(ecam_base + cfg_addr + offset, data);

}

/**
  @brief   This API programs the Memory Base and Memeory limit register of the Bus,
           Device and Function of Type1 Header
  @param   bus,dev,func   - Bus(8-bits), device(8-bits), function(8-bits)
           bar32_p_base   - 32 bit prefetchable memory base address
           bar32_np_base  - 32 bit non-prefetchable memory base address
           bar32_p_limit  - 32 bit prefetchable memory base limit
           bar32_np_limit - 32 bit non-prefetchable memory base limit
  @return  None
**/
static void
get_resource_base_32(uint32_t bus, uint32_t dev, uint32_t func, uint32_t bar32_p_base,
                     uint32_t bar32_np_base, uint32_t bar32_p_limit, uint32_t bar32_np_limit)
{
  uint32_t mem_bar_np;
  uint32_t mem_bar_p;

  /*Update the 32 bit NP-BAR start address for the next iteration*/
  if (bar32_np_base != g_bar32_np_start)
  {
      if ((g_bar32_np_start << 12) != 0)
          g_bar32_np_start  = (g_bar32_np_start & MEM_BASE32_LIM_MASK) + BAR_INCREMENT;
      if (bar32_np_limit == g_bar32_np_start)
          bar32_np_limit = bar32_np_limit - BAR_INCREMENT;
      pal_pci_cfg_read(bus, dev, func, NON_PRE_FET_OFFSET, &mem_bar_np);
      mem_bar_np = ((bar32_np_limit & MEM_BASE32_LIM_MASK) | mem_bar_np);
      pal_pci_cfg_write(bus, dev, func, NON_PRE_FET_OFFSET, mem_bar_np);
   }

  /*Update the 32 bit P-BAR start address for the next iteration*/
  if (bar32_p_base != g_bar32_p_start)
  {
      if ((g_bar32_p_start  << 12) != 0)
          g_bar32_p_start  = (g_bar32_p_start & MEM_BASE32_LIM_MASK) + BAR_INCREMENT;
      if (bar32_p_limit == g_bar32_p_start)
          bar32_p_limit = bar32_p_limit - BAR_INCREMENT;
      pal_pci_cfg_read(bus, dev, func, PRE_FET_OFFSET, &mem_bar_p);
      mem_bar_p = ((bar32_p_limit & MEM_BASE32_LIM_MASK) | mem_bar_p);
      pal_pci_cfg_write(bus, dev, func, PRE_FET_OFFSET, mem_bar_p);
   }
}

/**
  @brief   This API programs the Memory Base and Memeory limit register of the Bus,
           Device and Function of Type1 Header
  @param   bus,dev,func   - Bus(8-bits), device(8-bits), function(8-bits)
           bar64_p_base   - 64 bit prefetchable memory base address
           g_bar64_p_max  - 64 bit prefetchable memory base limit
  @return  None
**/
static void
get_resource_base_64(uint32_t bus, uint32_t dev, uint32_t func, uint64_t bar64_p_base, uint64_t g_bar64_p_max)
{

  /*Update the 64 bit BAR start address for the next iteration*/
  uint32_t bar64_p_lower32_base = (uint32_t)bar64_p_base;
  uint32_t bar64_p_upper32_base = bar64_p_base >> 32;

  uint32_t bar64_p_lower32_limit = (uint32_t)g_bar64_p_max;
  uint32_t bar64_p_upper32_limit = g_bar64_p_max >> 32;


  /*Obtain the memory base and memory limit*/
  bar64_p_lower32_base = REG_MASK_SHIFT(bar64_p_lower32_base);
  bar64_p_lower32_limit = REG_MASK_SHIFT(bar64_p_lower32_limit);
  uint32_t mem_bar_p = ((bar64_p_lower32_limit << 16) | bar64_p_lower32_base);

  /*Configure Memory base and Memory limit register*/
  if ((bar64_p_base != g_bar64_p_max) && (g_bar64_p_start <= g_bar64_p_max))
  {
      if ((g_bar64_p_start  << 12) != 0)
          g_bar64_p_start  = (g_bar64_p_start & MEM_BASE64_LIM_MASK) + BAR_INCREMENT;
      if (bar64_p_lower32_limit == g_bar64_p_start)
          bar64_p_lower32_limit = bar64_p_lower32_limit - BAR_INCREMENT;
      g_bar64_p_start = (g_bar64_p_start & MEM_BASE64_LIM_MASK) + BAR_INCREMENT;
      pal_pci_cfg_write(bus, dev, func, PRE_FET_OFFSET, mem_bar_p);
      pal_pci_cfg_write(bus, dev, func, PRE_FET_OFFSET + 4, bar64_p_upper32_base);
      pal_pci_cfg_write(bus, dev, func, PRE_FET_OFFSET + 8, bar64_p_upper32_limit);
  }
}

void
pal_pcie_rp_program_bar(uint32_t bus, uint32_t dev, uint32_t func)
{

  uint64_t bar_size, bar_upper_bits;
  uint32_t bar_reg_value, bar_lower_bits;
  uint32_t offset = BAR0_OFFSET;

  while(offset <= TYPE1_BAR_MAX_OFF)
  {
      pal_pci_cfg_read(bus, dev, func, offset, &bar_reg_value);
      if (BAR_REG(bar_reg_value) == BAR_64_BIT)
      {
          print(ACS_PRINT_INFO, "The BAR supports P_MEM 64-bit addr decoding capability\n", 0);

          /** BAR supports 64-bit address therefore, write all 1's
            *  to BARn and BARn+1 and identify the size requested
          **/
          pal_pci_cfg_write(bus, dev, func, offset, 0xFFFFFFF0);
          pal_pci_cfg_write(bus, dev, func, offset + 4, 0xFFFFFFFF);
          pal_pci_cfg_read(bus, dev, func, offset, &bar_lower_bits);
          bar_size = bar_lower_bits & BAR_MASK;

          pal_pci_cfg_read(bus, dev, func, offset + 4, &bar_reg_value);
          bar_upper_bits = bar_reg_value;
          bar_size = bar_size | (bar_upper_bits << 32 );

          bar_size = ~bar_size + 1;

          /**If BAR size is 0, then BAR not implemented, move to next BAR**/
          if (bar_size == 0)
          {
              offset = offset + 8;
              continue;
          }
          pal_pci_cfg_write(bus, dev, func, offset, g_rp_bar64_value);
          pal_pci_cfg_write(bus, dev, func, offset + 4, g_rp_bar64_value >> 32);
          offset = offset + 8;
      }

      else
      {
          print(ACS_PRINT_INFO, "The BAR supports P_MEM 32-bit addr decoding capability\n", 0);

          /**BAR supports 32-bit address. Write all 1's
           * to BARn and identify the size requested
          **/
          pal_pci_cfg_write(bus, dev, func, offset, 0xFFFFFFF0);
          pal_pci_cfg_read(bus, dev, func, offset, &bar_lower_bits);
          bar_reg_value = bar_lower_bits & BAR_MASK;
          bar_size = ~bar_reg_value + 1;

          /**If BAR size is 0, then BAR not implemented, move to next BAR**/
          if (bar_size == 0)
          {
              offset = offset + 4;
              continue;
          }
          pal_pci_cfg_write(bus, dev, func, offset, g_rp_bar32_value);
          print(ACS_PRINT_INFO, "Value written to BAR register is %x\n", g_rp_bar32_value);
          g_rp_bar32_value = g_rp_bar32_value + bar_size;
          offset = offset + 4;
      }
  }

}


/**
  @brief   This API programs all the BAR register in PCIe config space pointed by Bus,
           Device and Function for an End Point PCIe device
  @param   bus,dev,func - Bus(8-bits), device(8-bits) & function(8-bits)
  @return  None
**/
void pal_pcie_program_bar_reg(uint32_t bus, uint32_t dev, uint32_t func)
{

  uint64_t bar_size, bar_upper_bits;
  uint32_t bar_reg_value, bar_lower_bits;
  uint32_t offset = BAR0_OFFSET;
  uint32_t np_bar_size = 0;
  uint32_t p_bar_size = 0, p_bar64_size = 0;

  while(offset <= TYPE0_BAR_MAX_OFF)
  {
    pal_pci_cfg_read(bus, dev, func, offset, &bar_reg_value);

    if (BAR_MEM(bar_reg_value) == BAR_PRE_MEM)
    {
        if (BAR_REG(bar_reg_value) == BAR_64_BIT)
        {
            print(ACS_PRINT_INFO, "The BAR supports P_MEM 64-bit addr decoding capability\n", 0);

            /** BAR supports 64-bit address therefore, write all 1's
              *  to BARn and BARn+1 and identify the size requested
            **/
            pal_pci_cfg_write(bus, dev, func, offset, 0xFFFFFFF0);
            pal_pci_cfg_write(bus, dev, func, offset + 4, 0xFFFFFFFF);
            pal_pci_cfg_read(bus, dev, func, offset, &bar_lower_bits);
            bar_size = bar_lower_bits & BAR_MASK;

            pal_pci_cfg_read(bus, dev, func, offset + 4, &bar_reg_value);
            bar_upper_bits = bar_reg_value;
            bar_size = bar_size | (bar_upper_bits << 32 );

            bar_size = ~bar_size + 1;

            /**If BAR size is 0, then BAR not implemented, move to next BAR**/
            if (bar_size == 0)
            {
                offset = offset + 8;
                continue;
            }

            /** If p_bar64_size = 0 and bus number is same as bus of previous bus number,
              * then check if the current PCIe Device BAR size is greater than the
              * previous BAR size, if yes then add current BAR size to the updated start
              * address else add the previous BAR size to the updated start address
            **/
            if ((p_bar64_size == 0) && ((g_64_bus == bus)))
            {
                if (g_bar64_size < bar_size)
                    g_bar64_p_start = g_bar64_p_start + bar_size;
                else
                    g_bar64_p_start = g_bar64_p_start + g_bar64_size;
            }
            else if ((g_bar64_size < bar_size) && (p_bar64_size != 0))
                g_bar64_p_start = g_bar64_p_start + bar_size;

            else
                g_bar64_p_start = g_bar64_p_start + p_bar64_size;

            pal_pci_cfg_write(bus, dev, func, offset, g_bar64_p_start);
            pal_pci_cfg_write(bus, dev, func, offset + 4, g_bar64_p_start >> 32);

            print(ACS_PRINT_INFO, "Value written to BAR register is %llx\n", g_bar64_p_start);
            p_bar64_size = bar_size;
            g_bar64_size = bar_size;
            g_64_bus = bus;
            offset = offset + 8;

        }

        else
        {
            print(ACS_PRINT_INFO, "The BAR supports P_MEM 32-bit addr decoding capability\n", 0);

            /**BAR supports 32-bit address. Write all 1's
             * to BARn and identify the size requested
            **/
            pal_pci_cfg_write(bus, dev, func, offset, 0xFFFFFFF0);
            pal_pci_cfg_read(bus, dev, func, offset, &bar_lower_bits);
            bar_reg_value = bar_lower_bits & BAR_MASK;
            bar_size = ~bar_reg_value + 1;

            /**If BAR size is 0, then BAR not implemented, move to next BAR**/
            if (bar_size == 0)
            {
                offset = offset + 4;
                continue;
            }

            /** If p_bar_size = 0 and bus number is same as bus of previous bus number,
              * then check if the current PCIe Device BAR size is greater than the
              * previous BAR size, if yes then add current BAR size to the updated start
              * address else add the previous BAR size to the updated start address
            **/
            if ((p_bar_size == 0) && ((g_p_bus == bus)))
            {
                if (g_p_bar_size < bar_size)
                    g_bar32_p_start = g_bar32_p_start + bar_size;
                else
                    g_bar32_p_start = g_bar32_p_start + g_p_bar_size;
            }

            else if ((g_p_bar_size < bar_size) && (p_bar_size != 0))
                g_bar32_p_start = g_bar32_p_start + bar_size;

            else
                g_bar32_p_start = g_bar32_p_start + p_bar_size;

            pal_pci_cfg_write(bus, dev, func, offset, g_bar32_p_start);
            print(ACS_PRINT_INFO, "Value written to BAR register is %x\n", g_bar32_p_start);
            p_bar_size = bar_size;
            g_p_bar_size = bar_size;
            g_p_bus = bus;

            offset = offset + 4;
        }
    }

    else
    {
         print(ACS_PRINT_INFO, "The BAR supports NP_MEM 32-bit addr decoding capability\n", 0);

         /**BAR supports 32-bit address. Write all 1's
          * to BARn and identify the size requested
         **/
         pal_pci_cfg_write(bus, dev, func, offset, 0xFFFFFFF0);
         pal_pci_cfg_read(bus, dev, func, offset, &bar_lower_bits);
         bar_reg_value = bar_lower_bits & BAR_MASK;
         bar_size = ~bar_reg_value + 1;

         /**If BAR size is 0, then BAR not implemented, move to next BAR**/
         if (bar_size == 0)
         {
             if (BAR_REG(bar_lower_bits) == BAR_64_BIT)
                 offset = offset + 8;
             if (BAR_REG(bar_lower_bits) == BAR_32_BIT)
                 offset = offset + 4;
             continue;
         }

         /** If np_bar_size = 0 and bus number is same as bus of previous bus number,
           * then check if the current PCIe Device BAR size is greater than the
           * previous BAR size, if yes then add current BAR size to the updated start
           * address else add the previous BAR size to the updated start address
         **/
         if ((np_bar_size == 0) && ((g_np_bus == bus)))
         {
             if (g_np_bar_size < bar_size)
                 g_bar32_np_start = g_bar32_np_start + bar_size;
             else
                 g_bar32_np_start = g_bar32_np_start + g_np_bar_size;
         }

         else if ((g_np_bar_size < bar_size) && (np_bar_size != 0))
             g_bar32_np_start = g_bar32_np_start + bar_size;
         else
             g_bar32_np_start = g_bar32_np_start + np_bar_size;

         pal_pci_cfg_write(bus, dev, func, offset, g_bar32_np_start);
         print(ACS_PRINT_INFO, "Value written to BAR register is %x\n", g_bar32_np_start);
         np_bar_size = bar_size;
         g_np_bar_size = bar_size;
         g_np_bus = bus;

         pal_pci_cfg_read(bus, dev, func, offset, &bar_reg_value);
         if (BAR_REG(bar_reg_value) == BAR_64_BIT)
         {
             pal_pci_cfg_write(bus, dev, func, offset + 4, 0);
             offset = offset + 8;
         }
         if (BAR_REG(bar_reg_value) == BAR_32_BIT)
             offset = offset + 4;
    }
  }

     g_bar32_p_max = g_bar32_p_start;
     g_bar32_np_max = g_bar32_np_start;
     g_bar64_p_max =  g_bar64_p_start;
}

/**
  @brief   This API performs the PCIe bus enumeration
  @param   bus,sec_bus - Bus(8-bits), secondary bus (8-bits)
  @return  sub_bus - Subordinate bus
**/
uint32_t pal_pcie_enumerate_device(uint32_t bus, uint32_t sec_bus)
{

  uint32_t vendor_id;
  uint32_t header_value;
  uint32_t sub_bus = bus;
  uint32_t dev;
  uint32_t func;
  uint32_t class_code;
  uint32_t com_reg_value;
  uint32_t bar32_p_limit;
  uint32_t bar32_np_limit;

  if (bus == ((g_pcie_info_table->block[pcie_index].end_bus_num) + 1))
      return sub_bus;

  uint32_t bar32_p_base = g_bar32_p_start;
  uint32_t bar32_np_base = g_bar32_np_start;
  uint64_t bar64_p_base = g_bar64_p_start;

  for (dev = 0; dev < PCIE_MAX_DEV; dev++)
  {
    for (func = 0; func < PCIE_MAX_FUNC; func++)
    {
        pal_pci_cfg_read(bus, dev, func, 0, &vendor_id);
        if ((vendor_id == 0x0) || (vendor_id == 0xFFFFFFFF))
                continue;

        /*Skip Hostbridge configuration*/
        pal_pci_cfg_read(bus, dev, func, TYPE01_RIDR, &class_code);
        if ((((class_code >> CC_BASE_SHIFT) & CC_BASE_MASK) == HB_BASE_CLASS) &&
             (((class_code >> CC_SUB_SHIFT) & CC_SUB_MASK)) == HB_SUB_CLASS)
                continue;

        print(ACS_PRINT_INFO, "The Vendor id read is %x\n", vendor_id);
        print(ACS_PRINT_INFO, "Valid PCIe device found at %x %x %x\n ", bus, dev, func);
        pal_pci_cfg_read(bus, dev, func, HEADER_OFFSET, &header_value);
        if (PCIE_HEADER_TYPE(header_value) == TYPE1_HEADER)
        {
            print(ACS_PRINT_INFO, "TYPE1 HEADER found\n", 0);

            /* Enable memory access, Bus master enable and I/O access*/
            pal_pci_cfg_read(bus, dev, func, COMMAND_REG_OFFSET, &com_reg_value);
            pal_pci_cfg_write(bus, dev, func, COMMAND_REG_OFFSET, (com_reg_value | REG_ACC_DATA));

            pal_pci_cfg_write(bus, dev, func, BUS_NUM_REG_OFFSET, BUS_NUM_REG_CFG(0xFF, sec_bus, bus));
            pal_pci_cfg_write(bus, dev, func, NON_PRE_FET_OFFSET, ((g_bar32_np_start >> 16) & 0xFFF0));
            pal_pci_cfg_write(bus, dev, func, PRE_FET_OFFSET, ((g_bar32_p_start >> 16) & 0xFFF0));
            sub_bus = pal_pcie_enumerate_device(sec_bus, (sec_bus+1));
            pal_pci_cfg_write(bus, dev, func, BUS_NUM_REG_OFFSET, BUS_NUM_REG_CFG(sub_bus, sec_bus, bus));
            sec_bus = sub_bus + 1;

            /*Obtain the start memory base address and the final memory base address of 32 bit BAR*/
            bar32_p_limit = g_bar32_p_max;
            bar32_np_limit = g_bar32_np_max;
            get_resource_base_32(bus, dev, func, bar32_p_base, bar32_np_base, bar32_p_limit, bar32_np_limit);

            /*Obtain the start memory base address and the final memory base address of 64 bit BAR*/
            get_resource_base_64(bus, dev, func, bar64_p_base, g_bar64_p_max);


            pal_pcie_rp_program_bar(bus, dev, func);
            /*Update the base and limit values*/
            bar32_p_base = g_bar32_p_start;
            bar32_np_base = g_bar32_np_start;
            bar64_p_base = g_bar64_p_start;
        }

        if (PCIE_HEADER_TYPE(header_value) == TYPE0_HEADER)
        {
            print(ACS_PRINT_INFO, "END POINT found\n", 0);
            pal_pcie_program_bar_reg(bus, dev, func);
            sub_bus = sec_bus - 1;
        }
      }
    }

    return sub_bus;
}

/**
    @brief   This API clears the primary bus number configured in the
             Type1 Header.
             Note: This is done to make sure the hardware is compatible
                   with Linux enumeration.
    @param:  None
    @return: None
**/
void
pal_clear_pri_bus()
{
    uint32_t bus;
    uint32_t dev;
    uint32_t func;
    uint32_t bus_value;
    uint32_t header_value;
    uint32_t vendor_id;

    for (bus = 0; bus <= g_pcie_info_table->block[pcie_index].end_bus_num; bus++)
    {
        for (dev = 0; dev < PCIE_MAX_DEV; dev++)
        {
            for (func = 0; func < PCIE_MAX_FUNC; func++)
            {
                pal_pci_cfg_read(bus, dev, func, 0, &vendor_id);
                if ((vendor_id == 0x0) || (vendor_id == 0xFFFFFFFF))
                        continue;

                pal_pci_cfg_read(bus, dev, func, HEADER_OFFSET, &header_value);
                if (PCIE_HEADER_TYPE(header_value) == TYPE1_HEADER)
                {
                    pal_pci_cfg_read(bus, dev, func, BUS_NUM_REG_OFFSET, &bus_value);
                    bus_value = bus_value & PRI_BUS_CLEAR_MASK;
                    pal_pci_cfg_write(bus, dev, func, BUS_NUM_REG_OFFSET, bus_value);
                }
            }
         }
     }
}

static
void find_ecam_by_segment_buses(uint32_t segment, uint32_t start_bus, uint32_t end_bus)
{
  uint32_t i = 0;

  while (i < g_pcie_info_table->num_entries)
  {
    if ((segment == g_pcie_info_table->block[i].segment_num) &&
	(start_bus >= g_pcie_info_table->block[i].start_bus_num) &&
	(end_bus <= g_pcie_info_table->block[i].end_bus_num))
    {
      pcie_index = i;
      break;
    }
    i++;
  }
}

static
void pal_pcie_enumerate_roots(void)
{
  uint32_t root_index = 0;
  uint32_t segment, pri_bus, sec_bus, end_bus;

  print(ACS_PRINT_TEST, "\nStarting Root Enumeration \n", 0);

  while (root_index < g_pcie_root_table->num_entries)
  {
    segment = g_pcie_root_table->block[root_index].segment;
    pri_bus = g_pcie_root_table->block[root_index].start_bus_num;
    end_bus = g_pcie_root_table->block[root_index].end_bus_num;

    find_ecam_by_segment_buses(segment, pri_bus, end_bus);

    g_bar64_p_start  = g_pcie_root_table->block[root_index].bar64_val;
    g_rp_bar64_value = g_pcie_root_table->block[root_index].rp_bar64_val;
    g_bar32_np_start = g_pcie_root_table->block[root_index].bar32np_val;
    g_bar32_p_start  = g_pcie_root_table->block[root_index].bar32p_val;
    g_rp_bar32_value = g_pcie_root_table->block[root_index].rp_bar32_val;

    sec_bus = pri_bus + 1;
    pal_pcie_enumerate_device(pri_bus, sec_bus);
    root_index++;
  }

  pcie_index = 0;
  while (pcie_index < g_pcie_info_table->num_entries)
  {
      pal_clear_pri_bus();
      pcie_index++;
  }

  pcie_index = 0;
  enumerate = 0;
}

void pal_pcie_enumerate(void)
{
    uint32_t pri_bus, sec_bus;
    if (g_pcie_info_table->num_entries == 0)
    {
         print(ACS_PRINT_TEST, "\nSkipping Enumeration", 0);
         return;
    }

    if (g_pcie_root_table->num_entries > 0)
    {
         pal_pcie_enumerate_roots();
         return;
    }

    print(ACS_PRINT_TEST, "\nStarting Enumeration \n", 0);
    while (pcie_index < g_pcie_info_table->num_entries)
    {
       pri_bus = g_pcie_info_table->block[pcie_index].start_bus_num;
       sec_bus = pri_bus + 1;
       pal_pcie_enumerate_device(pri_bus, sec_bus);
       pal_clear_pri_bus();
       pcie_index++;
    }
    enumerate = 0;
    pcie_index = 0;
}

/**
    @brief   Returns the Bus, Dev, Function (in the form seg<<24 | bus<<16 | Dev <<8 | func)
             for a matching class code.
    @param   ClassCode  - is a 32bit value of format ClassCode << 16 | sub_class_code
    @param   StartBdf   - is 0     : start enumeration from Host bridge
                          is not 0 : start enumeration from the input segment, bus, dev
                          this is needed as multiple controllers with same class code are
                          potentially present in a system.
    @return  the BDF of the device matching the class code
**/
uint32_t
pal_pcie_get_bdf(uint32_t ClassCode, uint32_t StartBdf)
{

  uint32_t  Bus, InputBus, Seg = 0;
  uint32_t  Dev, InputDev;
  uint32_t  Func, InputFunc;
  uint32_t class_code;
  InputBus  = PCIE_EXTRACT_BDF_BUS(StartBdf);
  InputDev  = PCIE_EXTRACT_BDF_DEV(StartBdf);
  InputFunc = PCIE_EXTRACT_BDF_FUNC(StartBdf);

  for (Bus = InputBus; Bus < PLATFORM_BM_OVERRIDE_PCIE_MAX_BUS; Bus++)
  {
    for (Dev = InputDev; Dev < PCIE_MAX_DEV; Dev++)
    {
      for (Func = InputFunc; Func < PCIE_MAX_FUNC; Func++)
      {
        pal_pci_cfg_read(Bus, Dev, Func, TYPE01_RIDR, &class_code);
        if ((class_code >> CC_BASE_SHIFT) == (ClassCode >> 16))
        {
          if((class_code >> CC_SUB_SHIFT) == (ClassCode >> 8))
          {
           // Found our device
           // Return the BDF*/
            return (uint32_t)(PCIE_CREATE_BDF(Seg, Bus, Dev, Func));
          }
        }
      }
    }
  }
  return 0;
}

/**
  @brief  Increment the Device number (and Bus number if Dev num reaches 32) to the next valid value.
  @param  StartBdf  Segment/Bus/Dev/Func in the format created by PCIE_CREATE_BDF
  @return the new incremented BDF
**/
uint32_t
pal_increment_bus_dev(uint32_t StartBdf)
{

  uint32_t Seg = PCIE_EXTRACT_BDF_SEG(StartBdf);
  uint32_t Bus = PCIE_EXTRACT_BDF_BUS(StartBdf);
  uint32_t Dev = PCIE_EXTRACT_BDF_DEV(StartBdf);


  if (Dev != (PCIE_MAX_DEV-1)) {
      Dev++;
  } else {
      Bus++;
      Dev = 0;
  }

  return PCIE_CREATE_BDF(Seg, Bus, Dev, 0);
}

/**
  @brief  This API returns the Base Address Register value for a given BDF and index
  @param  bdf       - the device whose PCI Config space BAR needs to be returned.
  @param  bar_index - the '0' based BAR index identifying the correct 64-bit BAR
  @return the 64-bit BAR value
*/
uint64_t
pal_pcie_get_base(uint32_t bdf, uint32_t bar_index)
{
  uint32_t   Bus, Dev, Func;
  uint32_t   offset;
  uint32_t   bar_reg_lower_value;
  uint32_t   bar_reg_upper_value;
  uint64_t   bar_value, bar_upper_bits;

  Bus  = PCIE_EXTRACT_BDF_BUS(bdf);
  Dev  = PCIE_EXTRACT_BDF_DEV(bdf);
  Func = PCIE_EXTRACT_BDF_FUNC(bdf);

  offset = BAR0_OFFSET + (4*bar_index);


  pal_pci_cfg_read(Bus, Dev, Func, offset, &bar_reg_lower_value);

  bar_value = bar_reg_lower_value & BAR_MASK;

  if (BAR_REG(bar_reg_lower_value) == BAR_64_BIT)
  {
     pal_pci_cfg_read(Bus, Dev, Func, offset + 4, &bar_reg_upper_value);
     bar_upper_bits = bar_reg_upper_value;
     bar_value = bar_value | (bar_upper_bits << 32 );
  }

  print(ACS_PRINT_INFO, "value read from BAR 0x%llx\n", bar_value);

  return bar_value;

}

/**
    @brief   Returns the Bus, Dev, Function (in the form seg<<24 | bus<<16 | Dev <<8 | func)
             for a matching class code.
    @param   ClassCode  - is a 32bit value of format ClassCode << 16 | sub_class_code
    @param   StartBdf   - is 0     : start enumeration from Host bridge
                          is not 0 : start enumeration from the input segment, bus, dev
                          this is needed as multiple controllers with same class code are
                          potentially present in a system.
    @return  the BDF of the device matching the class code
**/
uint32_t
pal_pcie_get_bdf_wrapper(uint32_t class_code, uint32_t start_bdf)
{

  return pal_pcie_get_bdf(class_code, start_bdf);
}

/**
    @brief   Returns the Device ID of the bdf
    @param   bdf - Bus, Device and Function of the device
    @return  device_id on success
**/
void *
pal_pci_bdf_to_dev(uint32_t bdf)
{

  uint32_t bus;
  uint32_t dev;
  uint32_t func;
  uint32_t vendor_id, *device_id;

  bus  = PCIE_EXTRACT_BDF_BUS(bdf);
  dev  = PCIE_EXTRACT_BDF_DEV(bdf);
  func = PCIE_EXTRACT_BDF_FUNC(bdf);

  pal_pci_cfg_read(bus, dev, func, 0, &vendor_id);
  vendor_id = vendor_id >> DEVICE_ID_OFFSET;
  device_id = &vendor_id;

  return (void *)device_id;

}

/**
  @brief  This API is used as placeholder to check if the bdf
          obtained is valid or not

  @param  bdf
  @return 0 if bdf is valid else 1
**/
uint32_t
pal_pcie_check_device_valid(uint32_t bdf)
{

  /*Add BDFs to this function if PCIe tests
    need to be ignored for a BDF for any reason
  */
  (void) bdf;
  return 0;
}

/**
  @brief   Checks if System information is passed using Device Tree (DT)
           This api is also used to check if GIC/Interrupt Init ACS Code
           is used or not. In case of DT, ACS Code is used for INIT

  @param  None

  @return True/False
*/
uint32_t
pal_target_is_dt()
{
  return 0;
}

uint32_t
pal_bsa_pcie_enumerate()
{
  return 0; /* uefi takes care of it */
}



