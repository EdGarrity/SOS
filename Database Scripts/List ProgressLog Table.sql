/****** Script for SelectTopNRows command from SSMS  ******/
SELECT [ID]
      ,[Created_DTS]
      ,[Generation]
      ,[Generations_Completed_This_Session]
      ,[BestIndividual_ID]
      ,[BestIndividual_Training_Score]
      ,[BestIndividual_Training_Error]
      ,[Average_Training_Error]
      ,[BestIndividual_Test_Score]
      ,[Number_Of_Training_Cases]
      ,[Number_Of_Test_Cases]
	  ,DATALENGTH([Best_Genome]) AS [Genome Length]
	  ,LEN(CAST([Best_Genome] AS VARCHAR(MAX))) - LEN(REPLACE(CAST([Best_Genome] AS VARCHAR(MAX)), '}', '')) AS [Genome Points]
      ,[Best_Genome]
      ,[Population_Size]
      ,[Alternation_Rate]
      ,[Uniform_Mutation_Rate]
      ,[Example_Case_Max_Length]
      ,[Example_Case_Upper_Range]
  FROM [SOS].[dbo].[ProgressLog] (NOLOCK)
  order by id --desc
