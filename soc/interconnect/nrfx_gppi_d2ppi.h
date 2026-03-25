/*$$$LICENCE_NORDIC_STANDARD<2025>$$$*/

#ifndef NRFX_GPPI_D2PPI_H__
#define NRFX_GPPI_D2PPI_H__

#include <helpers/nrfx_gppi_routes.h>

/**
 * @defgroup nrfx_gppi_d2ppi GPPI structures specific to  Distributed (across multiple domains) DPPI (D2PPI) architecture.
 * @{
 * @ingroup nrfx_gppi
 *
 * @brief Structures for SoC specific GPPI API.
 */

/** @brief DPPI domain ID. */
typedef enum {
	NRFX_GPPI_DOMAIN_MCU  = 0, ///< MCU domain.
	NRFX_GPPI_DOMAIN_RAD  = 1, ///< Radio domain.
	NRFX_GPPI_DOMAIN_PERI = 2, ///< Peripheral domain.
	NRFX_GPPI_DOMAIN_LP   = 3, ///< Low power domain.
} nrfx_gppi_domain_id_t;

/** @brief Identification of a DPPI node in the system. */
typedef enum {
	NRFX_GPPI_NODE_DPPIC00,                               ///< DPPIC00 node
	NRFX_GPPI_NODE_DPPIC10,                               ///< DPPIC10 node
	NRFX_GPPI_NODE_DPPIC20,                               ///< DPPIC20 node
	NRFX_GPPI_NODE_DPPIC30,                               ///< DPPIC30 node
    NRFX_GPPI_NODE_DPPI_COUNT,                            ///< Number of DPPI nodes in the system.
	NRFX_GPPI_NODE_PPIB00_10 = NRFX_GPPI_NODE_DPPI_COUNT, ///< PPIB00-PPIB10 bridge node
	NRFX_GPPI_NODE_PPIB11_21,                             ///< PPIB11-PPIB21 bridge node
	NRFX_GPPI_NODE_PPIB01_20,                             ///< PPIB01-PPIB20 bridge node
	NRFX_GPPI_NODE_PPIB22_30,                             ///< PPIB22-PPIB30 bridge node
    NRFX_GPPI_NODE_COUNT                                  ///< Number of nodes in the system.
} nrfx_gppi_node_id_t;

/** @cond Driver internal data. */
const nrfx_gppi_route_t ***nrfx_gppi_route_map_get(void);
const nrfx_gppi_route_t *nrfx_gppi_routes_get(void);
const nrfx_gppi_node_t *nrfx_gppi_nodes_get(void);
void nrfx_gppi_channel_init(nrfx_gppi_node_id_t node_id, uint32_t ch_mask);
void nrfx_gppi_groups_init(nrfx_gppi_node_id_t node_id, uint32_t group_mask);
/** @endcond */

/** @} */

#endif // NRFX_GPPI_D2PPI_H__
