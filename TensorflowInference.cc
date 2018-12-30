/* Wearable Devices - Tensorflow library */



#include <cstdlib>
#include <unordered_set>
#include <string>
#include <fstream>
#include <map>

//#include <windows.h>

#include "TensorflowInference.h"

#pragma warning(disable:4996)
#pragma warning(disable:4018)
#pragma warning(disable:4554)
#pragma warning(disable:4244)

#define COMPILER_MSVC
#define NOMINMAX

#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/graph/algorithm.h"
#include "tensorflow/core/graph/graph.h"
#include "tensorflow/core/graph/graph_constructor.h"
#include "tensorflow/core/lib/strings/str_util.h"
#include "tensorflow/core/lib/strings/strcat.h"
#include "tensorflow/core/platform/env.h"
#include "tensorflow/core/platform/init_main.h"
#include "tensorflow/core/platform/platform.h"
#include "tensorflow/core/platform/types.h"
#include "tensorflow/core/public/session.h"
#include "tensorflow/core/util/command_line_flags.h"
#include "tensorflow/core/util/reporter.h"
#include "tensorflow/core/util/stat_summarizer.h"

using namespace tensorflow;
using namespace std;

using namespace Mudra::Computation;

class ComputationalModel;

map<const char *, ComputationalModel*> g_model;

template <class T>
void InitializeTensor(const vector<float>& initialization_values,
                    Tensor& input_tensor) {
  auto type_tensor = input_tensor.flat<T>();
  type_tensor = type_tensor.constant(0);
  if (!initialization_values.empty()) {
    for (int i = 0; i < initialization_values.size(); ++i) {
      type_tensor(i) = static_cast<T>(initialization_values[i]);
    }
  }
}

class ComputationalModel
{
    // Tensorflow objects
    unique_ptr<tensorflow::Session> m_session;
    unique_ptr<tensorflow::GraphDef> m_graphDef;
    
    // Model input parameters
    vector<string> m_modeInputNames;

    // Model output parameters
    vector<string> m_modelOutputNames;
    vector<tensorflow::Tensor> m_outputTensors;
    
    // Model run mode parameters
    RunOptions m_runOptions; 
    
	typedef pair<string, tensorflow::Tensor> TensorPair;
    
	// Inputs
    vector<TensorPair> m_inputTensors;
    
	// Logging
	shared_ptr<Logger> m_logger;
	
public:
    
    ComputationalModel(
		  const char *graph,
          int num_threads, 
		  const vector<pair<const char *, vector<long long>>> &modeInputs,
          const vector<const char *> &modelOutputNames, 
		  int loggerSeverity) :
		m_logger(make_shared<Logger>("Mudra", (Logger::Severity)loggerSeverity))
      {
		DebugMessage(m_logger) << "\nStart TensorFlow init function on " << graph;

        tensorflow::SessionOptions options;

		tensorflow::ConfigProto& config = options.config;
      
        if (num_threads > 0) {
            config.set_intra_op_parallelism_threads(num_threads);
        }
        
        // Read graph
        m_session.reset(NewSession(options));
        m_graphDef.reset(new GraphDef());
        
        // Tensorflow read graph from file
        Status s = ReadBinaryProto(Env::Default(), graph, m_graphDef.get());
        if (!s.ok()) {
			DebugMessage(m_logger) << "\nCould not create TensorFlow Graph: " << graph;
        }
		else
		{
			DebugMessage(m_logger) << "\nGraph " << graph << " read successfully! ";
		}
        
        // Create session 
        s = m_session->Create(*m_graphDef.get());
        InfoMessage(m_logger) << "\nCreate TensorFlow Session: " << s;

		// Init Tensors (same tensors are overwritten in real-time compute)
		for (auto& x : modeInputs) {
			TensorPair tensorPair;
			tensorPair.first = x.first;
			tensorPair.second = Tensor(DataType::DT_FLOAT, tensorflow::TensorShape(x.second));
			m_inputTensors.push_back(tensorPair);
		}

		for (auto& x : modelOutputNames) {
			m_modelOutputNames.push_back(x);
		}

        m_runOptions.set_trace_level(RunOptions::NO_TRACE);
    }
    
    void Run(const vector<vector<float>>& inputs, std::vector<std::vector<float>>& output)
    {
        //DebugMessage(m_logger) << "\nTensorFlowInference::Run started ";

		for (unsigned i = 0; i < m_inputTensors.size(); i++)
		{
			InitializeTensor<float>(inputs[i], m_inputTensors[i].second);
		}
        
        //Status s = m_session->Run(m_runOptions, m_inputTensors, m_modelOutputNames, {}, &m_outputTensors, {});
        //DebugMessage(m_logger) << "\nRun status = " << s;
        
        m_session->Run(m_runOptions, m_inputTensors, m_modelOutputNames, {}, &m_outputTensors, {});
        
		for (unsigned i = 0; i < m_outputTensors.size(); i++)
		{
			output[i].assign(m_outputTensors[i].flat<float>().data(),
				m_outputTensors[i].flat<float>().data() + m_outputTensors[i].flat<float>().size());
		}
    }
    
};

void InitTensorflowModel(
	const char *graphFileName,
	const vector<pair<const char *, vector<long long>>> &modeInputs,
	const vector<const char*> &modelOutputNames,
	int loggerSeverity)
{
    int num_threads = 2;

	g_model[graphFileName] = new ComputationalModel(graphFileName, num_threads, modeInputs, modelOutputNames, loggerSeverity);
}

    
void RunTensorflowModel(const char *graphFileName, const vector<vector<float>>& inputs, vector<vector<float>> &output)
{
    g_model[graphFileName]->Run(inputs, output);
}

