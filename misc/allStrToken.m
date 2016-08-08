%
% ALLSTRTOKEN returns cell array of tokens in S tokenized by any char in D
%
% T = allStrToken(S, D)

function T = allStrTokens(S, D)

	i = 1;
  while true
    [currT,S] = strtok(S,D);
		if numel(currT) == 0; break; end;
		T{i} = currT; i = i+1;
	end
