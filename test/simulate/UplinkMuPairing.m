% uplink MU pairing for Viettel case,32TRX RRU = 2x8x2
% input: candidateUE list; output: pairSetList
% candidateUE list = choose from {CS1,CS2}
% CS1: UEs have similar requested PRBs thus more likely to be paired
% CS2: UEs requested PRBs are implicilty associated with its PUSCH_SINR
%      thus CS2 will have much less paired UEs due to diverse requrestRBs
clear all;
close all; 
clc;
rng('default');
% prepare the pre-defined beam set for CS1 UEs
FileName = 'example_beamSet_32port_2x8x2_singlePol.csv';
TotalWeights = csvread(FileName);
[m, n] = size(TotalWeights);
beamweights = ones(m,n/2); % 16 beams in total, each as 8x1 BF vector
for ii = 0: n/2-1
    temp = TotalWeights(:,2*ii+1) + j*TotalWeights(:,2*ii+2) ;
    beamweights(:,ii+1) = temp/norm(temp,'fro');
end
% initialization, create CS1 UE list, each element of the list is a struct
numUEs_CS1 = 16;
reTrans = randi(1,1,numUEs_CS1);
PFterm = numUEs_CS1 * rand(1,numUEs_CS1);
PFterm = fliplr(sort(PFterm));
CS1 = cell(1,numUEs_CS1);
CandidateUserIndex = zeros(1,numUEs_CS1);
systemBandwidth = 273;
for ii = 1: numUEs_CS1
    CS1{ii}.msId = ii;
    CS1{ii}.reTransmission = reTrans(1,ii); % not used
    CS1{ii}.activityStatus = 1; % not used
    CS1{ii}.numLayers = 1;
    CS1{ii}.PF = PFterm(1,ii); % should be listed in desending order
    CS1{ii}.SINR_pusch = randi(30,1); % in dB value
    CS1{ii}.requestedRBs = randi([16 32],1);
    CS1{ii}.beam = beamweights(:,randi(256,1)); % complex-valued column vector    
end
% initialize another candidateUE list for internal comparison...
for ii = 1: numUEs_CS1
    CS2{ii}.msId = ii;
    CS2{ii}.reTransmission = reTrans(1,ii);% not used
    CS2{ii}.activityStatus = 1;% not used
    CS2{ii}.numLayers = 1;
    CS2{ii}.PF = PFterm(1,ii); % should be listed in desending order
    CS2{ii}.SINR_pusch = randi([1 25],1); % in dB value
    CS2{ii}.requestedRBs = min(128,ceil(exp(CS2{ii}.SINR_pusch/5)));
    CS2{ii}.beam = beamweights(:,randi(256,1)); % complex-valued column vector
 end
display('The intialization of candidateUE list is done!')
% pre-configured relevant thresholds
UlMuLowSinrThreshold = 3;  % in dB value
maxMuUserNum = 2;
maxMuLayerNum = 4;
corrThreshold = 0.3;
availablePRBnum = systemBandwidth;
%%%%%%%%%%%%%%% choose to use which candidate UE List %%%%%%%%%%%%%
candidateUEList = CS2;
disp('The input candidateUE list are:')
celldisp(candidateUEList);
disp('the beam weights of candidateUE are:')
for jj= 1:numUEs_CS1
    beam = candidateUEList{jj}.beam;
    sprintf('candidateUE %d beam:',candidateUEList{jj}.msId)
    disp(beam)
end
pairSetList = {}; % initialization for output
%%% internal check: across correlation factors and sinrDelta%%%
corrMatrix = zeros(numUEs_CS1, numUEs_CS1);
sinrMUdelta = zeros(numUEs_CS1, numUEs_CS1);
for jj = 1:numUEs_CS1
    rootUser = candidateUEList{jj};
    for kk = 1:numUEs_CS1
        secondUser = candidateUEList{kk};
        corrMatrix(jj,kk) = corr(rootUser, secondUser);
        [deltaA, deltaB] = calculateSINRdelta(rootUser, secondUser);
        sinrMUdelta(jj,kk) = deltaA;
    end
end
display('internal check for Corr factors and sinrMUdelta...!')
pcolor(corrMatrix)
title('correlation factors across every 2 users!');
xlabel('user index');
ylabel('user index');
colorbar;
figure;
pcolor(sinrMUdelta)
title('SINR degradation from SU to MU for every possible user pair!');
xlabel('user index');
ylabel('user index');
colorbar;
%%%  end of internal check %%%


% start user pairing process...
disp('begin MU pairing process...')
while size(candidateUEList)>0 % candidateUEList not empty
      tmpUser = candidateUEList{1};
      if(size(candidateUEList,2)==1)&&(tmpUser.requestedRBs <= availablePRBnum)
          % take the only user as SU user - makePairSU()
          pairset = makePairSet_SU(tmpUser);
          pairSetList = [pairSetList pairset];
          % remove this user out of candidate UE list
          candidateUEList(1)=[];
      else % check pairing possibility among multiple user candidates
          if(tmpUser.SINR_pusch > UlMuLowSinrThreshold)
              rootUser = tmpUser;
              % then try to find its pair in remaining candidate UE list
              candidateUEList(1) = [];
              % for each user in remaining candidate UE list,
              % check its SINR threshold, correlation factor, spectrum
              % efficiency, PRB ratio, port allocation,
              for s = 1:size(candidateUEList,2)
                  secondUser = candidateUEList{s};
                  [validity,sinrMU_rootUser,sinrMU_secondUser]= pair(rootUser, secondUser,UlMuLowSinrThreshold,corrThreshold,availablePRBnum);
                  if validity == 1
                      % make a pairset %%%%%%%%%%%%%%%%>>>>>>>>>>>>>>>
                      pairSet = makePairSet_MU(rootUser, secondUser,sinrMU_rootUser, sinrMU_secondUser);
                      % add the latest pairset into pairset list
                      pairSetList = [pairSetList pairSet];
                      availablePRBnum = availablePRBnum - pairSet.prb;
                      % remove second user from candidateUElist
                      candidateUEList(s)=[];
                      break;
                  elseif(secondUser.msId == candidateUEList{end}.msId)&&(tmpUser.requestedRBs <= availablePRBnum)
                      % take first user as SU user - makePairSU()
                      pairSet = makePairSet_SU(tmpUser);
                      pairSetList = [pairSetList pairSet];
                      availablePRBnum = availablePRBnum - pairSet.prb;
                      % remove this user out of candidate UE list
                      % candidateUEList(1)=[];
                  end
              end
          elseif(tmpUser.requestedRBs<=availablePRBnum)
              % take first user as SU user - makePairSU()
              pairSet = makePairSet_SU(tmpUser);
              pairSetList = [pairSetList pairSet];
              availablePRBnum = availablePRBnum - pairSet.prb;
              % remove this user out of candidate UE list
              candidateUEList(1)=[];
          else % drop this tmpUser - it cannot be scheduled in this TTI!
              candidateUEList(1)=[];
          end
      end
      
end
disp('pairing process done! output pairSet list are:');
celldisp(pairSetList);


function [pairSet] = makePairSet_MU(userA, userB,sinrMU_rootUser, sinrMU_secondUser)
   
   pairSet.ueNum = 2;
   pairSet. msId1 = userA.msId;
   pairSet. msId2 = userB.msId;
   pairSet.prb = userA.requestedRBs;
   pairSet.corrFactor = corr(userA, userB);
   pairSet.sinrMU_A = sinrMU_rootUser; % dB value
   pairSet.sinrMU_B = sinrMU_secondUser; % dB value
   pairSet.layerSum = 2;
end

function [pairSet] = makePairSet_SU(userA)

  pairSet.ueNum = 1;
  pairSet.msId1 = userA.msId;
  pairSet.msId2 = -1;
  pairSet.prb = userA.requestedRBs;
  pairSet.corrFactor = 0;
  pairSet.sinrMU_A = userA.SINR_pusch; % dB value
  pairSet.sinrMU_B = -100; % dB value
  pairSet.layerSum = 1;
end

function [successfulPair, sinrMU_userA, sinrMU_userB]=pair(rootUser, secondUser,UlMuLowSinrThreshold,corrThreshold,availablePRBnum)
% rootUser and secondUser are data structs,
% this function is try to check its correlation factor, SINR threshold,
% spectrum efficiency, PRB ratio and port allocation
% port allocation is ignored herein due to simple allocation in uplink
  if (secondUser.SINR_pusch <= UlMuLowSinrThreshold)
    successfulPair = 0;
  else
      corrFactor = corr(rootUser, secondUser);
      if (corrFactor < corrThreshold)
          % further check spectrum efficiency
          successfulPair = checkSpectrumEfficiency(rootUser, secondUser);
          if successfulPair == 1
              % get sinrMU values
              [sinrMU_userA, sinrMU_userB]=checkMuSinr(rootUser, secondUser);  
              % then further check PRB ratio
              [flag,PRBratio] = checkPRBratio(rootUser, secondUser);
              if (flag == 1)&&(rootUser.requestedRBs <= availablePRBnum)                  
                  successfulPair = 1;
              else
                  successfulPair = 0;
                  if(rootUser.requestedRBs > availablePRBnum)
                    sprintf('try pairing user %d and user %d failed as requestedRB:%d > availablePRBnum: %d!', rootUser.msId,secondUser.msId,rootUser.requestedRBs,availablePRBnum)    
                  else
                    sprintf('try pairing user %d and user %d failed, because of PRB ratio = %f!', rootUser.msId, secondUser.msId, PRBratio)
                  end
              end
          else
              successfulPair = 0;
              sprintf('try pairing user %d and user %d failed for the sake of Spectrum Efficiency!', rootUser.msId, secondUser.msId)
          end
      else
          successfulPair = 0;
          sprintf('try pairing user %d and user %d failed, due to corrFactor!', rootUser.msId, secondUser.msId)
      end    
  end
  
  if(successfulPair == 0)
      sinrMU_userA = -1;
      sinrMU_userB = -1;
  end
end

function [factor] = corr(userA, userB)
% check two user's correlation factor
   beam1 = userA.beam;
   beam2 = userB.beam;
   factor = norm(beam1'*beam2)/norm(beam1,'fro')/norm(beam2,'fro');
   factor = factor*factor;
   
end

function [sinrMU_A, sinrMU_B] = checkMuSinr(userA, userB)
        sinrSU_A = 10^(userA.SINR_pusch/10);% original value is dB value
        sinrSU_B = 10^(userB.SINR_pusch/10);% original value is dB value
        alpha_factor = 1.0;
        beamA = userA.beam;
        beamB = userB.beam;
        sinrMU_A = 1/(1/sinrSU_A + alpha_factor*((norm(beamA'*beamB,'fro')/norm(beamA'*beamA,'fro')))^2);
        sinrMU_A = 10*log10(sinrMU_A); % output dB value
        sinrMU_B = 1/(1/sinrSU_B + alpha_factor*((norm(beamB'*beamA,'fro')/norm(beamB'*beamB,'fro')))^2);   
        sinrMU_B = 10*log10(sinrMU_B); % output dB value
end

function [flag] = checkSpectrumEfficiency(userA, userB)
      [sinrMU_root, sinrMU_second] = checkMuSinr(userA, userB);
       sinr_deltaA = sinrMU_root - userA.SINR_pusch;
       sinr_deltaB = sinrMU_second - userB.SINR_pusch;
       sinrMU_root_linear = 10^(sinrMU_root/10);
       sinrMU_second_linear = 10^(sinrMU_second/10);
       sinrSU_root_linear = 10^(userA.SINR_pusch/10);
       sinrSU_second_linear = 10^(userB.SINR_pusch/10);
       SpectrumEfficiency_SU = (10*log2(1+sinrSU_root_linear) + 10*log2(1+sinrSU_second_linear))/2;% occupies two times PRB than MU
       SpectrumEfficiency_MU = 10*log2(1+sinrMU_root_linear) + 10*log2(1+sinrMU_second_linear);
       if(SpectrumEfficiency_MU > SpectrumEfficiency_SU)
           flag = 1;
       else 
           flag = 0;
           sprintf('SINR delta in pairing user %d and user %d are:  %f and %f respectively', userA.msId, userB.msId,sinr_deltaA, sinr_deltaB)
           sprintf('Spectrum Efficiency check for user %d and user %d is SU: %f Vs MU: %f',userA.msId, userB.msId,SpectrumEfficiency_SU,SpectrumEfficiency_MU)
       end     
end

function [flag, ratio] = checkPRBratio(userA, userB)       
       PRB_userA = userA.requestedRBs;  
       PRB_userB = userB.requestedRBs;       
       ratio = PRB_userB/PRB_userA;
       if (ratio>=0.9) &&(ratio<=1.1)
           flag = 1;
       else 
           flag = 0;
       end
end
function [sinrMU_deltaA, sinrMU_deltaB] = calculateSINRdelta(userA, userB)
         sinrSU_A = userA.SINR_pusch;
         sinrSU_B = userB.SINR_pusch;
         [sinrMU_A,sinrMU_B]= checkMuSinr(userA,userB);
         sinrMU_deltaA = sinrMU_A - sinrSU_A;
         sinrMU_deltaB = sinrMU_B - sinrSU_B;
end





















