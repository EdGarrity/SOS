/****** Script for SelectTopNRows command from SSMS  ******/
SELECT [ID]
      ,[Created_DTS]
      ,DATALENGTH([Genome]) as [Length]
      ,[Genome]
  FROM [SOS].[dbo].[Individuals] (nolock)
