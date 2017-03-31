/*
** google_translate.c for vm in /home/thibrex/Dropbox/delivery/CPE/CPE_2016_corewar/src/vm
** 
** Made by Thibaut Cornolti
** Login   <thibaut.cornolti@epitech.eu>
** 
** Started on  Thu Mar 30 16:21:36 2017 Thibaut Cornolti
** Last update Fri Mar 31 13:24:56 2017 Thibaut Cornolti
*/

#include <fcntl.h>
#include <unistd.h>
#include "vm.h"
#include "my.h"

static int	set_type(t_inst *inst, unsigned char code)
{
  code >>= 2;
  inst->arg[2].type = binary_to_type(code & 0x03);
  code >>= 2;
  inst->arg[1].type = binary_to_type(code & 0x03);
  code >>= 2;
  inst->arg[0].type = binary_to_type(code & 0x03);
  return (1);
}

static void	fill_arg(t_inst *inst, t_map *map, t_ptr *ptr)
{
  int		i;
  int		j;
  char		type_size;

  i = -1;
  while (++i < 3)
    {
      type_size = type_to_size(inst->inst, inst->arg[i].type);
      j = -1;
      while (++j < type_size)
	((char *) (&(inst->arg[i].arg)))[j] =
	  map->arena[(ptr->index_map + j) % MEM_SIZE];
      endian(&(inst->arg[i].arg), type_size);
      ptr->index_map += type_size;
      if (ptr->father->reg[0] == 2)
	dprintf(2, "arg %d:%d\n", i, inst->arg[i].arg);
    }
  dprintf(2, "\n");
}

int		translate(t_ptr *ptr, t_map *map)
{
  t_inst	inst;

  if (map->arena[PTR_INDEX] == 0)
    {
      ptr->live += 1;
      ptr->index_map += 1;
      return (0);
    }
  my_memset(&inst, 0, sizeof(t_inst));
  inst.inst = map->arena[PTR_INDEX];
  ptr->index_map += 1;
  if (!(inst.inst == 1 || inst.inst == 9 ||
	inst.inst == 12 || inst.inst == 15))
    ptr->index_map += set_type(&inst, map->arena[PTR_INDEX]);
  fill_arg(&inst, map, ptr);
  ptr->cycle = inst_to_time(inst.inst);
  return (0);
}