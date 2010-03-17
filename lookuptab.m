## Copyright (C) 2010 remington
## 
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2 of the License, or
## (at your option) any later version.
## 
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

## c:\lookuptab

## Author: remington <remington@M4300-01>
## Created: 2010-03-16

function [ z ] = lookuptab ()
	x = 1:255;
	y = round(245.^(x/255)-1) + 10;
	
	z = [y(1:end) y(end-1:-1:1)];
	
	step = 2;
	
	j = 0;
	fid = fopen('c:\exp.txt', 'w');
	for i = 1:step:length(z)
		j = j + 1;
		
		fprintf(fid, '%3u, ', z(i));
		if(mod(j, 8) == 0)
			fprintf(fid, '\n');
		end
	end
	fclose(fid);
		
	plot(z(1:step:end));

endfunction
