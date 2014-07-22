| **Name** | **Type**             | **Direction** | **Polarity** | **Description** |
|----------|----------------------|:-------------:|:------------:|-----------------|
| clk_i    | std_ulogic           | IN            | HIGH         |                 |
| rst_ni   | std_ulogic           | IN            | LOW          |                 |
| en_pi    | std_ulogic           | IN            | HIGH         |                 |
| count_o  | std_ulogic_vector[n] | OUT           | HIGH         |                 |
| tc_o     | std_ulogic           | OUT           | HIGH         |                 |


| **Name** | **Type** | **Default value** |
|----------|----------|-------------------|
| n        | natural  | 4                 |
| m        | natural  | 9                 |