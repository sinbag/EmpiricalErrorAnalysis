function out = ColectConvData (ns, nr, ofile, binfile, sstructs, istructs, atype)

nsamps = num2str(ns) ;
nreps  = num2str(nr) ;
delete(ofile);

nstypes = length(sstructs) ;
nitypes = length(istructs) ;

for i=1:nstypes
    stype = sstructs(i).stype ;
    sarg = sstructs(i).sarg ;
    dispstr = ['Collecting data using ' stype ' ' sarg '...'] ; 
    disp(dispstr) ;
    for j=1:nitypes
        itype = istructs(j).itype ;
        iarg = istructs(j).iarg ;
        dispstr = ['     on integrand ' itype ' ' iarg '...'] ; 
        disp(dispstr) ;
        if strcmp(itype,'Pbrt')
         spos = strfind(iarg, '$$SAMP$$') ;
         if spos<length(iarg)
             iarg = strrep(iarg, '$$SAMP$$', PBRTSamplerTypes(stype)) ;
         end
        end
        execstr = [binfile ' -S --stype ' stype ' ' sarg ' -I --itype ' itype ' ' iarg ' -A --atype ' atype ' --nsamps  ' nsamps ' --nreps ' nreps ' -G --ofile ' ofile]  ;
        system(execstr);
    end
end

% M = csvread([ofile '-' atype '-matlab.txt']) ;
% [h w] = size(M) ;
% [h w]
% if h*w==1
%     disp ('.') ;
%     testdbg = 1;
% end
% out.S = M(:, 1:w/2) ;
% out.V = M(:,w/2+1:w) ;


M = dlmread([ofile '-' atype '.txt']) ;
out.S = reshape(M(:,1)', size(ns,2) ,size(M,1)/size(ns,2))' ;
out.V = reshape(M(:,2)', size(ns,2), size(M,1)/size(ns,2))' ;

out.ns = ns ;
out.nr = nr ;
out.ofile = ofile; 
out.sstructs = sstructs ; 
out.istructs = istructs ;
out.atype = atype ;
end

function out = PBRTSamplerTypes(stype)
if strcmp(stype,'Jittered') out = 'stratified'; end 
if strcmp(stype,'Random') out = 'random'; end 
if ~length(out) error(['cannot run PBRT with ' stype ' sampler']) ; end
end