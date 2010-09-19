#pragma once

#include "PreInclude.h"
#include "NatureTime.h"
#include "BitSet.h"

/*
	base_ptr	rd_ptr		   wr_ptr
	|			|			   |
	|___________|___remaining__|____________________
	|_________used_size________|_______space_______|
	|_________________alloc_space__________________|
*/

#define M_Packet_Default_Size	2048
#define M_Packet_Inc_Size		512

namespace Cactus
{
	class COMMONUTIL_API OutOfBoundException : public std::exception
	{
	public:
		virtual const char *what() const throw()
		{	
			return "Read exceeds the size of the Packet buffer!";
		}
	};

	class COMMONUTIL_API PacketIsReadOnlyException : public std::exception
	{
	public:
		virtual const char *what() const throw()
		{	
			return "The Packet is Read Only!";
		}
	};

	class COMMONUTIL_API Packet : public Object
	{
	public:
		Packet(size_t size_ = M_Packet_Default_Size);
		Packet(const Packet &packet_);
		~Packet();

		// ������ֵ
		Packet& operator = (const Packet &packet_);

		// ����д��ָ������
		void				reset(bool reset_used_flag_ = false);

		// ׷��һ��Packet
		void				append(const Packet &packet_);

		// ��Packet���ݷָoutPkt���棬�������δ���귵��true
		// ���bTruncΪtrue���Ƴ��ָ��������
		bool				split(Packet& outPkt, size_t len_, bool bTrunc = false);

		// ������ָ��֮ǰ������
		void				truncate();

		// ���Ż�����
		bool				resize(size_t new_size_);

		// ����дָ��λ��
		void				set_used_size(size_t used_size_);

		// ��ȡ������ʼָ��
		const uint8*		get_base_ptr() const;
		uint8*				get_base_ptr();

		// ��ȡ��ָ��
		const uint8*		get_rd_ptr() const;
		uint8*				get_rd_ptr();

		// ��ȡдָ��
		const uint8*		get_wr_ptr() const;
		uint8*				get_wr_ptr();

		// ���ö�ָ��
		bool				set_rd_ptr(uint8* ptr);

		// ��ȡ������Ч��С����дָ��λ��
		size_t				get_size() const;

		// ��ȡʣ��ɶ������ݴ�С
		size_t				get_remaining() const;

		// ��Packetд������
		bool				writeInto(const void *pdata_, size_t len_);

		// ��Packet��������
		bool				readOut(void *pdata_, size_t len_, bool MoveReadPtr = true);

		// ��Packet����ת��ΪBase64��д��String
		void				readOutToBase64String(Cactus::String& str);

		// ��Base6�����Stringд��Packet
		void				writeIntoBase64String(const Cactus::String& str);

		/* put data */
		Packet& operator << (const int& v);
		Packet& operator << (const long& v);
		Packet& operator << (const uint& v);
		Packet& operator << (const int8& v);
		Packet& operator << (const uint8& v);
		Packet& operator << (const int16& v);
		Packet& operator << (const uint16& v);
		Packet& operator << (const int64& v);
		Packet& operator << (const uint64& v);
		Packet& operator << (const bool& v);
		Packet& operator << (const float& v);
		Packet& operator << (const double& v);
		Packet& operator << (const wchar_t& v);
		Packet& operator << (const Cactus::String& str);
		Packet& operator << (const Cactus::WString& str);
		Packet& operator << (const Packet& p);
		Packet& operator << (const NatureTime& t);


		/* get data */
		Packet& operator >> (int& v);
		Packet& operator >> (long& v);
		Packet& operator >> (uint& v);
		Packet& operator >> (int8& v);
		Packet& operator >> (uint8& v);
		Packet& operator >> (int16& v);
		Packet& operator >> (uint16& v);
		Packet& operator >> (int64& v);
		Packet& operator >> (uint64& v);
		Packet& operator >> (bool& v);
		Packet& operator >> (float& v);
		Packet& operator >> (double& v);
		Packet& operator >> (wchar_t& v);
		Packet& operator >> (Cactus::String& str);
		Packet& operator >> (Cactus::WString& str);
		Packet& operator >> (Packet& p);
		Packet& operator >> (NatureTime& t);


		/* peek data */
		template<class T>
		bool Peek(T& v)
		{
			return readOut((void*)&v, sizeof(v), false);
		}
		template<> bool Peek(Cactus::String& str);
		template<> bool Peek(Cactus::WString& str);
		template<> bool Peek(Packet& p);
		template<> bool Peek(NatureTime& t);


	protected:

		void			_init();

		/* ���ݰ����ÿռ��С */
		size_t			_alloc_size;

		/* �ռ����ʼλ�� */
		uint8*			_pbase;

		/* ʵ�����ݿ�Ĵ�С��Ҳ��дָ���λ�� */
		size_t			_used_size;

		/* ��ǰ��ָ���λ�� */
		uint8*			_ptr_read;
	};

	template<size_t bits>
	inline Cactus::Packet& operator << (Cactus::Packet& os, const BitSet<bits>& bt)
	{
		if (bt.any())
		{
			uint16 sz = bt.getwordsize();
			os << sz;

			for (uint16 t = 0; t < sz; ++t)
			{
				os << bt.getword(t);
			}
		}
		else
		{
			uint16 sz = 0;
			os << sz;
		}

		return os;
	}

	template<size_t bits>
	inline Cactus::Packet& operator >> (Cactus::Packet& is, BitSet<bits>& bt)
	{
		uint16 sz = 0;
		is >> sz;

		if (sz == 0)
		{
			bt.reset();
		}
		else
		{
			for (uint16 t = 0; t < sz; ++t)
			{
				uint val = 0;
				is >> val;

				bt.setword(t, val);
			}
		}

		return is;
	}

	template<size_t bits>
	inline bool Peek(Cactus::Packet& is, BitSet<bits>& bt)
	{
		uint8* rd_ptr = is.get_rd_ptr();

		is >> bt;

		return is.set_rd_ptr(rd_ptr);
	}

}