function pltout = PlotConvData (dout) 

fsz = 20; % fontsize

ni = length(dout.istructs) ;
ns = length(dout.sstructs) ;
h = size(dout.S,1) ;
S = dout.S ;
V = dout.V ;

cmap = lines(ns) ; % colours
marker = 'o+*^><.' ; % marker styles

maxlegwidth = 15 ;
mt = 1.5 ; % marker thickness of plotted points


hf1 = figure('PaperPosition',[0 0 14 14],'PaperSize',[14 14]);
hf2 = figure('PaperPosition',[0 0 26 14],'PaperSize',[25 14]);

LineEqs = zeros(h, 2) ;

avg = [0 0] ;
for i=1:h
    snum = (floor((i-1)/ni)) + 1;
    inum = mod(i-1,ni)+1 ;
    mk=marker(mod(inum,length(marker))) ;
    col = cmap(snum,:) ;

    x = S(i,:) ; 
    y = V(i,:) ;
    P = polyfit(log(x),log(y),1) ;
    LineEqs(i,:) = P ;
    
    figure(hf1) ; % Convergence Plot
    loglog(x, y, ['--' mk], 'color', col) ;
    hold on; 
    loglog(x, exp(polyval(P,log(x))), 'color', col) ;
    
    figure(hf2) ; % Scatter Plot
    avg = avg + P ;
    sp(i) = plot(P(1), (P(2)), 'Color', col, 'linewidth', mt) ;
    hold on; 
    sp2(i) = plot(P(1), (P(2)), mk, 'Color', 'black', 'linewidth', mt) ;
    plot(P(1), (P(2)), mk, 'Color', col, 'linewidth', mt) ;
    
    % set up legend strings
    sa = dout.sstructs(snum).sarg ;
    legstr{snum}=[ dout.sstructs(snum).stype ' ' sa(1:min(length(sa), maxlegwidth)) '...'];
    spidx(snum) = i ;
    ia = dout.istructs(inum).iarg ;
    if isfield(dout.istructs(inum), 'leg')
        ia = dout.istructs(inum).leg ;
    end
    legstr2{inum}=[dout.istructs(inum).itype ' '  ia(1:min(length(ia), maxlegwidth)) '...'] ;
    spidx2(inum) = i ;
end
avg = avg/h ;

figure(hf1); 
set(gca, 'fontsize', fsz) ;
xlabel('log-#samples')
ylabel(['log-' dout.atype]) ;
title('Convergence plot') ;
print([dout.ofile '-convs.pdf'], '-dpdf') ;

figure(hf2) ;
set(gca, 'fontsize', fsz) ;
xlabel('convergence rate (of squared error) ')
ylabel(['log-' dout.atype ' (1-sample estimator)']) ;
dummsh1 = plot(avg(1), avg(2), 'color', [1 1 1]) ;
dummsh2 = plot(avg(1), avg(2), 'color', [1 1 1]) ;
dummsh3 = plot(avg(1), avg(2), 'color', [1 1 1]) ;
title('Scatter plot') ;
legend([dummsh1 sp(spidx) dummsh2 dummsh3 sp2(spidx2)], ['    Sampling (colour) ' legstr ' ' ' Integrands (marker style) ' legstr2], 'location', 'NorthEastOutside', 'Fontsize', fsz) ;
print([dout.ofile '-scatter.pdf'], '-dpdf') ;


close all ;
end