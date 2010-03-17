# A simple Octave script for generating
# and viewing lookup tables quickly

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
