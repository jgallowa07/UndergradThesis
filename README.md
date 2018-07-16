# UndergradThesis
contains the source for my undergraduate thesis

`TexPaper` - contains all the Tex source

`figures` - All plots and figures referenced in paper

`Bench` - All benchmark simulations / scripts for non - neutral comparisons

**Abstract**

Large-scale, forward-moving evolutionary simulations are starting to play a key role in research surrounding populations genetics. Simulations can help biologists understand data we observe from natural populations  
in fields such as molecular ecology, evolutionary genetics, and conservation biology.
However, the magnitude of these simulations is limited by the current capabilities of the hardware they are running on. 
This draws interest in exploring methods that make the results of large-scale simulations more feasible. 
Earlier this year, a strategy was introduced allowing simulations to avoid tracking and propagating neutral 
mutations as a consequence of storing the entire genealogical history of sampled genotypes.
In this thesis, we use succinct tree sequences, introduced by \textbf{msprime}, to
explore and implement the method of genealogical tree sequence recording (\textit{TreeSeq})
in forward-moving evolutionary simulations.
We then analyze the runtime performance gain of one to two orders of magnitude as a result of 
implementing this strategy into a popular evolutionary framework, \textbf{SLiM}.
Finally, we explain the workflow, algorithms, and data structures 
behind the implementation.
