#ifndef AudioSink_hxx
#define AudioSink_hxx

#include "Processing.hxx"
#include "AudioInPort.hxx"

#include <sstream>

namespace CLAM
{
	class AudioSink : public Processing
	{
	public:
		struct Port
		{
			float* mFloatBuffer;
			double* mDoubleBuffer;
			unsigned mBufferSize;
			AudioInPort* mPort;

			//resize needs a default constructor
			Port()
			: mFloatBuffer(0), mDoubleBuffer(0), mBufferSize(0), mPort(0)
			{
			}

			explicit Port(AudioInPort* p)
			: mFloatBuffer(0), mDoubleBuffer(0), mBufferSize(0), mPort(p)
			{
			}
		};
		typedef std::vector<Port> Ports;

	private:
		class Config : public ProcessingConfig
		{
			DYNAMIC_TYPE_USING_INTERFACE( Config, 1, ProcessingConfig );
			DYN_ATTRIBUTE( 0, public, int, NSinks);
		protected:
			void DefaultInit()
			{
				AddAll();
				UpdateData();
				SetNSinks(1);
			};
			void LoadFrom(Storage & storage)
			{
				ProcessingConfig::LoadFrom(storage);
				if (not HasNSinks())
				{
					AddNSinks();
					UpdateData();
					SetNSinks(1);
				}
				}
			};

	private:
		Config _config;
		Ports _ports;

	public:
		AudioSink(const ProcessingConfig & config=Config())
		{
			//After being dropped it is ready to run as it does not need any configuration at all
			//SetExecState(Ready);
			Configure( config );
			ResizePorts(1);
		}

		~AudioSink()
		{
			for (unsigned port = 0; port < _ports.size(); ++port)
				delete _ports[port].mPort;
		}

		/// @deprecated Delegated to SetExternalBuffer
		void SetFrameAndHopSize(const int val, unsigned index)
		{
			CLAM_ASSERT(index < _ports.size(), "AudioInPort index out of range");
			Port& port = _ports[index];
			port.mPort->SetSize(val);
			port.mPort->SetHop(val);
		}

		void SetExternalBuffer(float* buf, unsigned nframes, unsigned index);
		void SetExternalBuffer(double* buf, unsigned nframes, unsigned index);

		bool Do();

		const char* GetClassName() const { return "AudioSink";}

		const ProcessingConfig & GetConfig() const
		{
			return _config;
		}

		bool ConcreteConfigure(const ProcessingConfig& config)
		{
			CopyAsConcreteConfig(_config, config);
			unsigned sinks = _config.GetNSinks();

			ResizePorts(sinks);

				return true;
		}

		Ports& GetPorts() { return _ports; }

	private:
		std::string const Portname(unsigned port) const
		{
			std::ostringstream os;
			os << port + 1; //to make ports one based (when viewed in jack)
			return os.str();
		}

		void ResizePorts(unsigned sinks)
		{
			if (sinks == _ports.size())
				return;

			for (unsigned port = sinks; port < _ports.size(); ++port)
				delete _ports[port].mPort;

			unsigned oldSize = _ports.size();
			_ports.resize(sinks);

			for (unsigned port = oldSize; port < sinks; ++port)
				_ports[port] = Port(new AudioInPort(Portname(port), this));
		}
	};

} //namespace CLAM

#endif

