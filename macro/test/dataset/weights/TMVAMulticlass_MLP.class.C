// Class: ReadMLP
// Automatically generated by MethodBase::MakeClass
//

/* configuration options =====================================================

#GEN -*-*-*-*-*-*-*-*-*-*-*- general info -*-*-*-*-*-*-*-*-*-*-*-

Method         : MLP::MLP
TMVA Release   : 4.2.1         [262657]
ROOT Release   : 6.08/00       [395264]
Creator        : ywang
Date           : Thu Aug 10 16:56:06 2017
Host           : Linux ilcsoft_build_sl6.desy.de 2.6.32-573.12.1.el6.x86_64 #1 SMP Tue Dec 15 08:24:23 CST 2015 x86_64 x86_64 x86_64 GNU/Linux
Dir            : /afs/desy.de/user/y/ywang/Code/MyTest2/macro/test
Training events: 4000
Analysis type  : [Classification]


#OPT -*-*-*-*-*-*-*-*-*-*-*-*- options -*-*-*-*-*-*-*-*-*-*-*-*-

# Set by User:
NCycles: "1000" [Number of training cycles]
HiddenLayers: "N+5,5" [Specification of hidden layer architecture]
NeuronType: "tanh" [Neuron activation function type]
EstimatorType: "MSE" [MSE (Mean Square Estimator) for Gaussian Likelihood or CE(Cross-Entropy) for Bernoulli Likelihood]
V: "False" [Verbose output (short form of "VerbosityLevel" below - overrides the latter one)]
H: "False" [Print method-specific help message]
TestRate: "5" [Test for overtraining performed at each #th epochs]
# Default:
RandomSeed: "1" [Random seed for initial synapse weights (0 means unique seed for each run; default value '1')]
NeuronInputType: "sum" [Neuron input function type]
VerbosityLevel: "Default" [Verbosity level]
VarTransform: "None" [List of variable transformations performed before training, e.g., "D_Background,P_Signal,G,N_AllClasses" for: "Decorrelation, PCA-transformation, Gaussianisation, Normalisation, each for the given class of events ('AllClasses' denotes all events of all classes, if no class indication is given, 'All' is assumed)"]
CreateMVAPdfs: "False" [Create PDFs for classifier outputs (signal and background)]
IgnoreNegWeightsInTraining: "False" [Events with negative weights are ignored in the training (but are included for testing and performance evaluation)]
TrainingMethod: "BP" [Train with Back-Propagation (BP), BFGS Algorithm (BFGS), or Genetic Algorithm (GA - slower and worse)]
LearningRate: "2.000000e-02" [ANN learning rate parameter]
DecayRate: "1.000000e-02" [Decay rate for learning parameter]
EpochMonitoring: "False" [Provide epoch-wise monitoring plots according to TestRate (caution: causes big ROOT output file!)]
Sampling: "1.000000e+00" [Only 'Sampling' (randomly selected) events are trained each epoch]
SamplingEpoch: "1.000000e+00" [Sampling is used for the first 'SamplingEpoch' epochs, afterwards, all events are taken for training]
SamplingImportance: "1.000000e+00" [ The sampling weights of events in epochs which successful (worse estimator than before) are multiplied with SamplingImportance, else they are divided.]
SamplingTraining: "True" [The training sample is sampled]
SamplingTesting: "False" [The testing sample is sampled]
ResetStep: "50" [How often BFGS should reset history]
Tau: "3.000000e+00" [LineSearch "size step"]
BPMode: "sequential" [Back-propagation learning mode: sequential or batch]
BatchSize: "-1" [Batch size: number of events/batch, only set if in Batch Mode, -1 for BatchSize=number_of_events]
ConvergenceImprove: "1.000000e-30" [Minimum improvement which counts as improvement (<0 means automatic convergence check is turned off)]
ConvergenceTests: "-1" [Number of steps (without improvement) required for convergence (<0 means automatic convergence check is turned off)]
UseRegulator: "False" [Use regulator to avoid over-training]
UpdateLimit: "10000" [Maximum times of regulator update]
CalculateErrors: "False" [Calculates inverse Hessian matrix at the end of the training to be able to calculate the uncertainties of an MVA value]
WeightRange: "1.000000e+00" [Take the events for the estimator calculations from small deviations from the desired value to large deviations only over the weight range]
##


#VAR -*-*-*-*-*-*-*-*-*-*-*-* variables *-*-*-*-*-*-*-*-*-*-*-*-

NVar 4
var1                          var1                          var1                          var1                                                            'F'    [-4.05916023254,3.58076572418]
var2                          var2                          var2                          Variable 2                                                      'F'    [-3.68905711174,3.78774046898]
var3                          var3                          var3                          Variable 3                    units                             'F'    [-3.61478614807,4.56402540207]
var4                          var4                          var4                          Variable 4                    units                             'F'    [-4.84856987,5.04116535187]
NSpec 0


============================================================================ */

#include <vector>
#include <cmath>
#include <string>
#include <iostream>

#ifndef IClassifierReader__def
#define IClassifierReader__def

class IClassifierReader {

 public:

   // constructor
   IClassifierReader() : fStatusIsClean( true ) {}
   virtual ~IClassifierReader() {}

   // return classifier response
   virtual double GetMvaValue( const std::vector<double>& inputValues ) const = 0;

   // returns classifier status
   bool IsStatusClean() const { return fStatusIsClean; }

 protected:

   bool fStatusIsClean;
};

#endif

class ReadMLP : public IClassifierReader {

 public:

   // constructor
   ReadMLP( std::vector<std::string>& theInputVars ) 
      : IClassifierReader(),
        fClassName( "ReadMLP" ),
        fNvars( 4 ),
        fIsNormalised( false )
   {      
      // the training input variables
      const char* inputVars[] = { "var1", "var2", "var3", "var4" };

      // sanity checks
      if (theInputVars.size() <= 0) {
         std::cout << "Problem in class \"" << fClassName << "\": empty input vector" << std::endl;
         fStatusIsClean = false;
      }

      if (theInputVars.size() != fNvars) {
         std::cout << "Problem in class \"" << fClassName << "\": mismatch in number of input values: "
                   << theInputVars.size() << " != " << fNvars << std::endl;
         fStatusIsClean = false;
      }

      // validate input variables
      for (size_t ivar = 0; ivar < theInputVars.size(); ivar++) {
         if (theInputVars[ivar] != inputVars[ivar]) {
            std::cout << "Problem in class \"" << fClassName << "\": mismatch in input variable names" << std::endl
                      << " for variable [" << ivar << "]: " << theInputVars[ivar].c_str() << " != " << inputVars[ivar] << std::endl;
            fStatusIsClean = false;
         }
      }

      // initialize min and max vectors (for normalisation)
      fVmin[0] = 0;
      fVmax[0] = 0;
      fVmin[1] = 0;
      fVmax[1] = 0;
      fVmin[2] = 0;
      fVmax[2] = 0;
      fVmin[3] = 0;
      fVmax[3] = 0;

      // initialize input variable types
      fType[0] = 'F';
      fType[1] = 'F';
      fType[2] = 'F';
      fType[3] = 'F';

      // initialize constants
      Initialize();

   }

   // destructor
   virtual ~ReadMLP() {
      Clear(); // method-specific
   }

   // the classifier response
   // "inputValues" is a vector of input values in the same order as the 
   // variables given to the constructor
   double GetMvaValue( const std::vector<double>& inputValues ) const;

 private:

   // method-specific destructor
   void Clear();

   // common member variables
   const char* fClassName;

   const size_t fNvars;
   size_t GetNvar()           const { return fNvars; }
   char   GetType( int ivar ) const { return fType[ivar]; }

   // normalisation of input variables
   const bool fIsNormalised;
   bool IsNormalised() const { return fIsNormalised; }
   double fVmin[4];
   double fVmax[4];
   double NormVariable( double x, double xmin, double xmax ) const {
      // normalise to output range: [-1, 1]
      return 2*(x - xmin)/(xmax - xmin) - 1.0;
   }

   // type of input variable: 'F' or 'I'
   char   fType[4];

   // initialize internal variables
   void Initialize();
   double GetMvaValue__( const std::vector<double>& inputValues ) const;

   // private members (method specific)

   double ActivationFnc(double x) const;
   double OutputActivationFnc(double x) const;

   int fLayers;
   int fLayerSize[4];
   double fWeightMatrix0to1[10][5];   // weight matrix from layer 0 to 1
   double fWeightMatrix1to2[6][10];   // weight matrix from layer 1 to 2
   double fWeightMatrix2to3[4][6];   // weight matrix from layer 2 to 3

   double * fWeights[4];
};

inline void ReadMLP::Initialize()
{
   // build network structure
   fLayers = 4;
   fLayerSize[0] = 5; fWeights[0] = new double[5]; 
   fLayerSize[1] = 10; fWeights[1] = new double[10]; 
   fLayerSize[2] = 6; fWeights[2] = new double[6]; 
   fLayerSize[3] = 4; fWeights[3] = new double[4]; 
   // weight matrix from layer 0 to 1
   fWeightMatrix0to1[0][0] = -0.38778850670751;
   fWeightMatrix0to1[1][0] = 1.77075352763869;
   fWeightMatrix0to1[2][0] = -0.495887946364255;
   fWeightMatrix0to1[3][0] = -1.07287478074003;
   fWeightMatrix0to1[4][0] = -1.27493222222149;
   fWeightMatrix0to1[5][0] = -0.232068331688551;
   fWeightMatrix0to1[6][0] = -0.0100184567840386;
   fWeightMatrix0to1[7][0] = -0.504687461356197;
   fWeightMatrix0to1[8][0] = -0.41456998867676;
   fWeightMatrix0to1[0][1] = 0.344484078249326;
   fWeightMatrix0to1[1][1] = 0.982416309163123;
   fWeightMatrix0to1[2][1] = -0.0419129952225741;
   fWeightMatrix0to1[3][1] = -0.703283427306373;
   fWeightMatrix0to1[4][1] = -0.768397705168322;
   fWeightMatrix0to1[5][1] = -0.166873265311981;
   fWeightMatrix0to1[6][1] = 0.551561437331797;
   fWeightMatrix0to1[7][1] = -2.49983402360569;
   fWeightMatrix0to1[8][1] = 3.22680407145307;
   fWeightMatrix0to1[0][2] = 0.384100424207592;
   fWeightMatrix0to1[1][2] = 0.872517592990635;
   fWeightMatrix0to1[2][2] = -0.0871975702555971;
   fWeightMatrix0to1[3][2] = -1.52887190551762;
   fWeightMatrix0to1[4][2] = -0.168358013186753;
   fWeightMatrix0to1[5][2] = 1.50557472966874;
   fWeightMatrix0to1[6][2] = -1.17198963689223;
   fWeightMatrix0to1[7][2] = -0.0740450592612006;
   fWeightMatrix0to1[8][2] = -0.157477458717336;
   fWeightMatrix0to1[0][3] = -0.381185082592116;
   fWeightMatrix0to1[1][3] = -3.17908830387991;
   fWeightMatrix0to1[2][3] = -0.422381779159728;
   fWeightMatrix0to1[3][3] = 2.54234963914718;
   fWeightMatrix0to1[4][3] = 1.72413680540312;
   fWeightMatrix0to1[5][3] = -1.03648108259518;
   fWeightMatrix0to1[6][3] = 0.75010463707696;
   fWeightMatrix0to1[7][3] = -0.120045773739285;
   fWeightMatrix0to1[8][3] = 0.00520098351826005;
   fWeightMatrix0to1[0][4] = -2.23811315621248;
   fWeightMatrix0to1[1][4] = 0.299577800759728;
   fWeightMatrix0to1[2][4] = 0.173737774048086;
   fWeightMatrix0to1[3][4] = 2.01992950842827;
   fWeightMatrix0to1[4][4] = 2.47067034525462;
   fWeightMatrix0to1[5][4] = 2.69933718728504;
   fWeightMatrix0to1[6][4] = 1.24979124792121;
   fWeightMatrix0to1[7][4] = 4.37768351492713;
   fWeightMatrix0to1[8][4] = -3.6655386905835;
   // weight matrix from layer 1 to 2
   fWeightMatrix1to2[0][0] = -2.47375060347935;
   fWeightMatrix1to2[1][0] = 1.84237698308281;
   fWeightMatrix1to2[2][0] = -0.915246344719473;
   fWeightMatrix1to2[3][0] = 0.742030598942647;
   fWeightMatrix1to2[4][0] = 1.54286295830321;
   fWeightMatrix1to2[0][1] = 1.04760275173647;
   fWeightMatrix1to2[1][1] = 0.251274036343028;
   fWeightMatrix1to2[2][1] = 0.133311422450511;
   fWeightMatrix1to2[3][1] = -0.362651699534128;
   fWeightMatrix1to2[4][1] = -1.26560770951767;
   fWeightMatrix1to2[0][2] = -0.542093135020647;
   fWeightMatrix1to2[1][2] = -0.60714945425257;
   fWeightMatrix1to2[2][2] = -0.429759617883957;
   fWeightMatrix1to2[3][2] = 1.49053600605261;
   fWeightMatrix1to2[4][2] = 0.186321270572984;
   fWeightMatrix1to2[0][3] = -1.26885413818901;
   fWeightMatrix1to2[1][3] = 0.403278533237819;
   fWeightMatrix1to2[2][3] = -2.37515789916025;
   fWeightMatrix1to2[3][3] = -0.682549634822121;
   fWeightMatrix1to2[4][3] = 0.895671716449018;
   fWeightMatrix1to2[0][4] = 1.00468665089631;
   fWeightMatrix1to2[1][4] = 0.00294619057952214;
   fWeightMatrix1to2[2][4] = -0.858376459553123;
   fWeightMatrix1to2[3][4] = 2.31562800007637;
   fWeightMatrix1to2[4][4] = 0.39209065116205;
   fWeightMatrix1to2[0][5] = 0.453890573142123;
   fWeightMatrix1to2[1][5] = -1.65086032608904;
   fWeightMatrix1to2[2][5] = -1.38809189733852;
   fWeightMatrix1to2[3][5] = 2.00030576315926;
   fWeightMatrix1to2[4][5] = 1.28885335051608;
   fWeightMatrix1to2[0][6] = 0.737008121249608;
   fWeightMatrix1to2[1][6] = -2.07776319325285;
   fWeightMatrix1to2[2][6] = -0.395007874498507;
   fWeightMatrix1to2[3][6] = 0.71934456144467;
   fWeightMatrix1to2[4][6] = 0.0157924737451731;
   fWeightMatrix1to2[0][7] = 1.1303787673988;
   fWeightMatrix1to2[1][7] = 0.245023395607365;
   fWeightMatrix1to2[2][7] = 1.37767650460826;
   fWeightMatrix1to2[3][7] = -2.94220491047599;
   fWeightMatrix1to2[4][7] = -1.04119716294614;
   fWeightMatrix1to2[0][8] = 0.230803202575464;
   fWeightMatrix1to2[1][8] = 0.693532198172888;
   fWeightMatrix1to2[2][8] = 1.05733862485925;
   fWeightMatrix1to2[3][8] = -1.97011105977166;
   fWeightMatrix1to2[4][8] = -0.897528569439193;
   fWeightMatrix1to2[0][9] = 0.953931221485957;
   fWeightMatrix1to2[1][9] = -1.26759017153591;
   fWeightMatrix1to2[2][9] = 2.74759900060607;
   fWeightMatrix1to2[3][9] = -0.656795897499413;
   fWeightMatrix1to2[4][9] = -1.23312229444889;
   // weight matrix from layer 2 to 3
   fWeightMatrix2to3[0][0] = 0.809374089520293;
   fWeightMatrix2to3[1][0] = -0.217255510142443;
   fWeightMatrix2to3[2][0] = -0.452803002823739;
   fWeightMatrix2to3[3][0] = -0.50457845423497;
   fWeightMatrix2to3[0][1] = 0.782281118310111;
   fWeightMatrix2to3[1][1] = -0.665086851180649;
   fWeightMatrix2to3[2][1] = -1.10864578434138;
   fWeightMatrix2to3[3][1] = 0.171136991984684;
   fWeightMatrix2to3[0][2] = 0.0224037450653728;
   fWeightMatrix2to3[1][2] = -0.514585812191245;
   fWeightMatrix2to3[2][2] = 0.498674855353497;
   fWeightMatrix2to3[3][2] = -0.0063816407743405;
   fWeightMatrix2to3[0][3] = 0.0129312070692756;
   fWeightMatrix2to3[1][3] = 0.048805104141024;
   fWeightMatrix2to3[2][3] = 0.463802128086056;
   fWeightMatrix2to3[3][3] = -0.526403437932214;
   fWeightMatrix2to3[0][4] = 0.586629132427303;
   fWeightMatrix2to3[1][4] = -0.56516260388251;
   fWeightMatrix2to3[2][4] = -0.0193992692312397;
   fWeightMatrix2to3[3][4] = -0.00243559497633667;
   fWeightMatrix2to3[0][5] = 0.529319697003467;
   fWeightMatrix2to3[1][5] = -0.466670469159831;
   fWeightMatrix2to3[2][5] = -0.694783758016973;
   fWeightMatrix2to3[3][5] = 1.17756234424668;
}

inline double ReadMLP::GetMvaValue__( const std::vector<double>& inputValues ) const
{
   if (inputValues.size() != (unsigned int)fLayerSize[0]-1) {
      std::cout << "Input vector needs to be of size " << fLayerSize[0]-1 << std::endl;
      return 0;
   }

   for (int l=0; l<fLayers; l++)
      for (int i=0; i<fLayerSize[l]; i++) fWeights[l][i]=0;

   for (int l=0; l<fLayers-1; l++)
      fWeights[l][fLayerSize[l]-1]=1;

   for (int i=0; i<fLayerSize[0]-1; i++)
      fWeights[0][i]=inputValues[i];

   // layer 0 to 1
   for (int o=0; o<fLayerSize[1]-1; o++) {
      for (int i=0; i<fLayerSize[0]; i++) {
         double inputVal = fWeightMatrix0to1[o][i] * fWeights[0][i];
         fWeights[1][o] += inputVal;
      }
      fWeights[1][o] = ActivationFnc(fWeights[1][o]);
   }
   // layer 1 to 2
   for (int o=0; o<fLayerSize[2]-1; o++) {
      for (int i=0; i<fLayerSize[1]; i++) {
         double inputVal = fWeightMatrix1to2[o][i] * fWeights[1][i];
         fWeights[2][o] += inputVal;
      }
      fWeights[2][o] = ActivationFnc(fWeights[2][o]);
   }
   // layer 2 to 3
   for (int o=0; o<fLayerSize[3]; o++) {
      for (int i=0; i<fLayerSize[2]; i++) {
         double inputVal = fWeightMatrix2to3[o][i] * fWeights[2][i];
         fWeights[3][o] += inputVal;
      }
      fWeights[3][o] = OutputActivationFnc(fWeights[3][o]);
   }

   return fWeights[3][0];
}

double ReadMLP::ActivationFnc(double x) const {
   // hyperbolic tan
   return tanh(x);
}
double ReadMLP::OutputActivationFnc(double x) const {
   // identity
   return x;
}
   
// Clean up
inline void ReadMLP::Clear() 
{
   // clean up the arrays
   for (int lIdx = 0; lIdx < 4; lIdx++) {
      delete[] fWeights[lIdx];
   }
}
   inline double ReadMLP::GetMvaValue( const std::vector<double>& inputValues ) const
   {
      // classifier response value
      double retval = 0;

      // classifier response, sanity check first
      if (!IsStatusClean()) {
         std::cout << "Problem in class \"" << fClassName << "\": cannot return classifier response"
                   << " because status is dirty" << std::endl;
         retval = 0;
      }
      else {
         if (IsNormalised()) {
            // normalise variables
            std::vector<double> iV;
            iV.reserve(inputValues.size());
            int ivar = 0;
            for (std::vector<double>::const_iterator varIt = inputValues.begin();
                 varIt != inputValues.end(); varIt++, ivar++) {
               iV.push_back(NormVariable( *varIt, fVmin[ivar], fVmax[ivar] ));
            }
            retval = GetMvaValue__( iV );
         }
         else {
            retval = GetMvaValue__( inputValues );
         }
      }

      return retval;
   }
