
/*
 * Copyright (C) 2014-2015 Galois, Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License** as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.

 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
    .section ".start"
    .globl exception_vector_table

@ 5 low-order zero bits, i.e., 32-byte aligned
.align 5
exception_vector_table:
    b		unused
    b		undef
    b		FreeRTOS_SWI_Handler
    b		pabt
    b		dabt
    b		unused2
    b		FreeRTOS_IRQ_Handler
    b		firq_handler

unused:
    b       .
undef:
    b       .
svc:
    b       .
pabt:
    b       .
dabt:
    b       .
unused2:
    b       .
irq_handler:
    b       .
firq_handler:
    b       .


